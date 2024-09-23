//
// Created by PeterXi on 2024/8/10.
//

#include <assert.h>
#include "ReadThread.h"

ReadThread::ReadThread(VideoState *is)
        : XThread("ReadThread", 10, URGENT)
        ,mediaState(is){

}

ReadThread::~ReadThread() {

}

void ReadThread::onThreadRun(uint32_t now) {

    if (mediaState->abort_request) {
        stopThread();
    }
#ifdef FFP_MERGE
    if (mediaState->paused != mediaState->last_paused) {
            mediaState->last_paused = mediaState->paused;
            if (mediaState->paused)
                mediaState->read_pause_return = av_read_pause(mediaState->ic);
            else
                av_read_play(mediaState->ic);
        }
#endif
#if CONFIG_RTSP_DEMUXER || CONFIG_MMSH_PROTOCOL
    if (mediaState->paused &&
                (!strcmp(mediaState->ic->iformat->name, "rtsp") ||
                 (mediaState->ic->pb && !strncmp(mediaState->input_filename, "mmsh:", 5)))) {
            /* wait 10 ms to avoid trying to get another packet */
            /* XXX: horrible */
            sleep(10);
            continue;
        }
#endif
    if (mediaState->seek_req) {
        int64_t seek_target = mediaState->seek_pos;
        int64_t seek_min    = mediaState->seek_rel > 0 ? seek_target - mediaState->seek_rel + 2: INT64_MIN;
        int64_t seek_max    = mediaState->seek_rel < 0 ? seek_target - mediaState->seek_rel - 2: INT64_MAX;
// FIXME the +-2 is due to rounding being not done in the correct direction in generation
//      of the seek_pos/seek_rel variables

        mediaState->player->buffering(true);
        mediaState->player->msg_queue->putEmptyMessage(FFP_MSG_BUFFERING_UPDATE,0,0);
        int ret = avformat_seek_file(mediaState->ic, -1, seek_min, seek_target, seek_max, mediaState->seek_flags);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR,
                   "%s: error while seeking\n", mediaState->filename.c_str());
        } else {
            if (mediaState->audio_stream >= 0) {
                mediaState->audioq->flush();
                mediaState->audioq->put(flush_pkt);
            }
            if (mediaState->subtitle_stream >= 0) {
                mediaState->subtitleq->flush();
                mediaState->subtitleq->put(flush_pkt);
            }
            if (mediaState->video_stream >= 0) {
                if (mediaState->node_vdec) {
                    mediaState->node_vdec->func_flush();
                }
                mediaState->videoq->flush();
                mediaState->videoq->put(flush_pkt);
            }
            if (mediaState->seek_flags & AVSEEK_FLAG_BYTE) {
                mediaState->extclk->set_clock(NAN, 0);
            } else {
                mediaState->extclk->set_clock(seek_target / (double)AV_TIME_BASE, 0);
            }

            mediaState->latest_seek_load_serial = mediaState->videoq->serial;
            mediaState->latest_seek_load_start_at = av_gettime();
        }
        mediaState->dcc.current_high_water_mark_in_ms = mediaState->dcc.first_high_water_mark_in_ms;
        mediaState->seek_req = false;
        mediaState->queue_attachments_req = 1;
        mediaState->eof = 0;
#ifdef FFP_MERGE
        if (mediaState->paused)
                mediaState->player->pause(false);
#endif
        if (mediaState->auto_resume) {
            mediaState->pause_req = 0;
            if (mediaState->packet_buffering)
                mediaState->buffering_on = true;
            mediaState->auto_resume = 0;
            mediaState->player->pause(false);
        }
        if (mediaState->pause_req) { mediaState->player->pause(false); }
        mediaState->player->msg_queue->putEmptyMessage(FFP_MSG_SEEK_COMPLETE, av_rescale(seek_target, 1000, AV_TIME_BASE), ret);
        mediaState->player->buffering(true);
    }
    if (mediaState->queue_attachments_req) {
        if (mediaState->video_st && (mediaState->video_st->disposition & AV_DISPOSITION_ATTACHED_PIC)) {
            AVPacket copy;
            if (av_packet_ref(&copy, &mediaState->video_st->attached_pic) < 0) { LOGE("av_packet_ref FAIL"); }
            mediaState->videoq->put(&copy);
            mediaState->videoq->put_nullpacket(mediaState->video_stream);
        }
        mediaState->queue_attachments_req = 0;
    }

    /* if the queue are full, no need to read more */
    if (mediaState->infinite_buffer<1 && !mediaState->seek_req &&
        #ifdef FFP_MERGE
        (mediaState->audioq.size + mediaState->videoq.size + mediaState->subtitleq.size > MAX_QUEUE_SIZE
        #else
        (mediaState->audioq->size + mediaState->videoq->size + mediaState->subtitleq->size > mediaState->dcc.max_buffer_size
         #endif
         || (stream_has_enough_packets(mediaState->audio_st, mediaState->audio_stream, mediaState->audioq, MIN_FRAMES)
                && stream_has_enough_packets(mediaState->video_st, mediaState->video_stream, mediaState->videoq, MIN_FRAMES)
                && stream_has_enough_packets(mediaState->subtitle_st, mediaState->subtitle_stream, mediaState->subtitleq, MIN_FRAMES)))) {
        if (!mediaState->eof) {
            mediaState->player->buffering(false);
        }
        /* wait 10 ms */
        //todo
        return;
    }
    if ((!mediaState->paused || mediaState->completed) &&
        (!mediaState->audio_st || (mediaState->auddec->finished == mediaState->audioq->serial && mediaState->audio_fq->frame_queue_nb_remaining() == 0)) &&
        (!mediaState->video_st || (mediaState->viddec->finished == mediaState->videoq->serial && mediaState->video_fq->frame_queue_nb_remaining() == 0))) {
        if (mediaState->loop != 1 && (!mediaState->loop || --mediaState->loop)) {
            //stream_seek(is, ffp->start_time != AV_NOPTS_VALUE ? ffp->start_time : 0, 0, 0);
            if (!mediaState->seek_req) {
                mediaState->seek_pos = mediaState->start_time != AV_NOPTS_VALUE ? mediaState->start_time:0;
                mediaState->seek_rel = 0;
                mediaState->seek_flags &= ~AVSEEK_FLAG_BYTE;
                mediaState->seek_req = 1;
            }
        } else if (mediaState->autoexit) {
//            ret = AVERROR_EOF;
            LOGE("mediaState->autoexit AVERROR_EOF");
            stopThread();
        } else {
            mediaState->stat->track_statistic(mediaState);
            if (mediaState->completed) {
                av_log(mediaState, AV_LOG_INFO, "ffp_toggle_buffering: eof\n");
                // infinite wait may block shutdown
//                while(!mediaState->abort_request && !mediaState->seek_req)
//                    SDL_CondWaitTimeout(is->continue_read_thread, wait_mutex, 100);
//                SDL_UnlockMutex(wait_mutex);
                if (!mediaState->abort_request)
                    return;
            } else {
                mediaState->completed = 1;
                mediaState->auto_resume = 0;

                // TODO: 0 it's a bit early to notify complete here
                mediaState->player->buffering(false);
                mediaState->player->pause(true);
                if (mediaState->error) {
                    av_log(mediaState, AV_LOG_INFO, "ffp_toggle_buffering: error: %d\n", mediaState->error);
                    mediaState->player->msg_queue->putEmptyMessage(FFP_MSG_ERROR);
                } else {
                    av_log(mediaState, AV_LOG_INFO, "ffp_toggle_buffering: completed: OK\n");
                    mediaState->player->msg_queue->putEmptyMessage(FFP_MSG_COMPLETED);
                }
            }
        }
    }
    AVPacket pkt1, *pkt = &pkt1;
    pkt->flags = 0;
    int ret = av_read_frame(mediaState->ic, pkt);
    if (ret < 0) {
        int pb_eof = 0;
        int pb_error = 0;
        if ((ret == AVERROR_EOF || avio_feof(mediaState->ic->pb)) && !mediaState->eof) {
            pb_eof = 1;
            // check error later
        }
        if (mediaState->ic->pb && mediaState->ic->pb->error) {
            pb_eof = 1;
            pb_error = mediaState->ic->pb->error;
        }
        if (ret == AVERROR_EXIT) {
            pb_eof = 1;
            pb_error = AVERROR_EXIT;
        }

        if (pb_eof) {
            if (mediaState->video_stream >= 0)
                mediaState->videoq->put_nullpacket(mediaState->video_stream);
            if (mediaState->audio_stream >= 0)
                mediaState->audioq->put_nullpacket(mediaState->audio_stream);
            if (mediaState->subtitle_stream >= 0)
                mediaState->subtitleq->put_nullpacket(mediaState->subtitle_stream);
            mediaState->eof = 1;
        }
        if (pb_error) {
            if (mediaState->video_stream >= 0)
                mediaState->videoq->put_nullpacket(mediaState->video_stream);
            if (mediaState->audio_stream >= 0)
                mediaState->audioq->put_nullpacket(mediaState->audio_stream);
            if (mediaState->subtitle_stream >= 0)
                mediaState->subtitleq->put_nullpacket(mediaState->subtitle_stream);
            mediaState->eof = 1;
            mediaState->error = pb_error;
            av_log(mediaState, AV_LOG_ERROR, "av_read_frame error: %x(%c,%c,%c,%c): %s\n", mediaState->error,
                   (char) (0xff & (mediaState->error >> 24)),
                   (char) (0xff & (mediaState->error >> 16)),
                   (char) (0xff & (mediaState->error >> 8)),
                   (char) (0xff & (mediaState->error)),
                   VideoState::get_error_string(mediaState->error));
            // break;
        } else {
            mediaState->error = 0;
        }
        if (mediaState->eof) {
            mediaState->player->buffering(false);
//            SDL_Delay(100);
        }
        mediaState->stat->track_statistic(mediaState);
        return;
    } else {
        mediaState->eof = 0;
    }

    if (pkt->flags & AV_PKT_FLAG_DISCONTINUITY) {
        if (mediaState->audio_stream >= 0) {
            mediaState->audioq->put(flush_pkt);
        }
        if (mediaState->subtitle_stream >= 0) {
            mediaState->subtitleq->put(flush_pkt);
        }
        if (mediaState->video_stream >= 0) {
            mediaState->videoq->put(flush_pkt);
        }
    }

    /* check if packet is in play range specified by user, then queue, otherwise discard */
    int64_t stream_start_time = mediaState->ic->streams[pkt->stream_index]->start_time;
    int64_t pkt_ts = pkt->pts == AV_NOPTS_VALUE ? pkt->dts : pkt->pts;
    int pkt_in_play_range = mediaState->duration == AV_NOPTS_VALUE ||
                        (pkt_ts - (stream_start_time != AV_NOPTS_VALUE ? stream_start_time : 0)) *
                        av_q2d(mediaState->ic->streams[pkt->stream_index]->time_base) -
                        (double)(mediaState->start_time != AV_NOPTS_VALUE ? mediaState->start_time : 0) / 1000000
                        <= ((double)mediaState->duration / 1000000);
    if (pkt->stream_index == mediaState->audio_stream && pkt_in_play_range) {
        mediaState->audioq->put(pkt);
    } else if (pkt->stream_index == mediaState->video_stream && pkt_in_play_range
               && !(mediaState->video_st && (mediaState->video_st->disposition & AV_DISPOSITION_ATTACHED_PIC))) {
        mediaState->videoq->put(pkt);
    } else if (pkt->stream_index == mediaState->subtitle_stream && pkt_in_play_range) {
        mediaState->subtitleq->put(pkt);
    } else {
        av_packet_unref(pkt);
    }
    mediaState->stat->track_statistic(mediaState);

    if (mediaState->packet_buffering) {
        int64_t io_tick_counter = SDL_GetTickHR();
        if (abs((int)(io_tick_counter - prev_io_tick_counter)) > BUFFERING_CHECK_PER_MILLISECONDS) {
            prev_io_tick_counter = io_tick_counter;
            mediaState->player->checkbuffering();
        }
    }
}

void ReadThread::onCreate() {
    XThread::onCreate();
    mediaState->ic->interrupt_callback.callback = VideoState::decode_interrupt_cb;
    mediaState->ic->interrupt_callback.opaque = mediaState;

    //format
    if (!av_dict_get(mediaState->format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE)) {
        av_dict_set(&mediaState->format_opts, "scan_all_pmts", "1", AV_DICT_DONT_OVERWRITE);
        scan_all_pmts_set = 1;
    }
    if (av_stristart(mediaState->filename.c_str(), "rtmp", NULL) ||
        av_stristart(mediaState->filename.c_str(), "rtsp", NULL)) {
        // There is total different meaning for 'timeout' option in rtmp
        av_log(mediaState, AV_LOG_WARNING, "remove 'timeout' option for rtmp.\n");
        av_dict_set(&mediaState->format_opts, "timeout", NULL, 0);
    }

    //other format name???todo
    errCode = avformat_open_input(&mediaState->ic,mediaState->filename.c_str(),mediaState->iformat,&mediaState->format_opts);
    if (errCode < 0) {
        LOGE("ReadThread::onCreate avformat_open_input err");
    }

    if (scan_all_pmts_set)
        av_dict_set(&mediaState->format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE);

    if ((t = av_dict_get(mediaState->format_opts, "", NULL, AV_DICT_IGNORE_SUFFIX))) {
        av_log(NULL, AV_LOG_ERROR, "Option %s not found.\n", t->key);
#ifdef FFP_MERGE
        ret = AVERROR_OPTION_NOT_FOUND;
#endif
    }

    if (mediaState->genpts) {
        mediaState->ic->flags |= AVFMT_FLAG_GENPTS;
    }

    av_format_inject_global_side_data(mediaState->ic);


    //stream info
    if (mediaState->find_stream_info) {
        AVDictionary **opts = setup_find_stream_info_opts(mediaState->ic, mediaState->codec_opts);
        int orig_nb_streams = mediaState->ic->nb_streams;

        do {
            if (av_stristart(mediaState->filename.c_str(),"data:", nullptr) && orig_nb_streams >0) {
                int i =0;
                for (i = 0; i < orig_nb_streams; ++i) {
                    AVStream *streamTemp = mediaState->ic->streams[i];
                    if (!streamTemp || !streamTemp->codecpar || streamTemp->codecpar->profile == FF_PROFILE_UNKNOWN) {
                        break;
                    }
                }
                if (i == orig_nb_streams) {
                    break;
                }
                errCode = avformat_find_stream_info(mediaState->ic, opts);
            }
        }while (0);
        mediaState->player->msg_queue->putEmptyMessage(FFP_MSG_FIND_STREAM_INFO);

        for (int i = 0; i < orig_nb_streams; ++i) {
            av_dict_free(&opts[i]);
        }
        av_freep(&opts);

        if (errCode <0) {
            av_log(NULL, AV_LOG_WARNING,
                   "%s: could not find codec parameters\n", mediaState->filename.c_str());
        }
    }

    if (mediaState->ic->pb) {
        mediaState->ic->pb->eof_reached =0; // FIXME hack, ffplay maybe should not use avio_feof() to test for the end
    }
    if (mediaState->seek_by_bytes) {
        mediaState->seek_by_bytes = !!(mediaState->ic->iformat->flags & AVFMT_TS_DISCONT) && strcmp("ogg", mediaState->ic->iformat->name);
    }

    mediaState->max_frame_duration = (mediaState->ic->iformat->flags & AVFMT_TS_DISCONT) ? 10.0 : 3600.0;
    mediaState->max_frame_duration = 10.0;  //???
    av_log(mediaState->player, AV_LOG_INFO, "max_frame_duration: %.3f\n", mediaState->max_frame_duration);


#ifdef FFP_MERGE
    if (!window_title && (t = av_dict_get(ic->metadata, "title", NULL, 0)))
        window_title = av_asprintf("%s - %s", t->value, input_filename);

#endif

    /* if seeking requested, we execute it */
    if (mediaState->start_time != AV_NOPTS_VALUE) {
        int64_t timeStamp = mediaState->start_time;
        /* add the stream start time */
        if (mediaState->ic->start_time != AV_NOPTS_VALUE) {
            timeStamp = mediaState->ic->start_time;
        }
        int ret = avformat_seek_file(mediaState->ic,-1,INT64_MIN,timeStamp,INT64_MAX,0);
        if (ret <0) {
            av_log(NULL, AV_LOG_WARNING, "%s: could not seek to position %0.3f\n",
                   mediaState->filename.c_str(),(double)timeStamp / AV_TIME_BASE);
        }
    }
    mediaState->realtime = mediaState->is_realtime();

    if (mediaState->show_status)
        av_dump_format(mediaState->ic, 0, mediaState->filename.c_str(), 0);


    int video_stream_count = 0;
    int h264_stream_count = 0;
    int first_h264_stream = -1;
    int st_index[AVMEDIA_TYPE_NB];
    for (int i = 0; i < mediaState->ic->nb_streams; ++i) {
        AVStream * stream = mediaState->ic->streams[i];
        enum AVMediaType type = stream->codecpar->codec_type;
        stream->discard = AVDISCARD_ALL;
        if (type >= 0 && mediaState->wanted_stream_spec[type] && st_index[type] == -1) {
            if (avformat_match_stream_specifier(mediaState->ic, stream, mediaState->wanted_stream_spec[type]) > 0) {
                st_index[type] = i;
            }
        }
        // choose first h264

        if (type == AVMEDIA_TYPE_VIDEO) {
            enum AVCodecID codec_id = stream->codecpar->codec_id;
            video_stream_count++;
            if (codec_id == AV_CODEC_ID_H264) {
                h264_stream_count++;
                if (first_h264_stream < 0)
                    first_h264_stream = i;
            }
        }
    }
    if (video_stream_count > 1 && st_index[AVMEDIA_TYPE_VIDEO] < 0) {
        st_index[AVMEDIA_TYPE_VIDEO] = first_h264_stream;
        av_log(NULL, AV_LOG_WARNING, "multiple video stream found, prefer first h264 stream: %d\n", first_h264_stream);
    }
    if (!mediaState->video_disable)
        st_index[AVMEDIA_TYPE_VIDEO] =
                av_find_best_stream(mediaState->ic, AVMEDIA_TYPE_VIDEO,
                                    st_index[AVMEDIA_TYPE_VIDEO], -1, NULL, 0);
    if (!mediaState->audio_disable)
        st_index[AVMEDIA_TYPE_AUDIO] =
                av_find_best_stream(mediaState->ic, AVMEDIA_TYPE_AUDIO,
                                    st_index[AVMEDIA_TYPE_AUDIO],
                                    st_index[AVMEDIA_TYPE_VIDEO],
                                    NULL, 0);
    if (!mediaState->video_disable && !mediaState->subtitle_disable)
        st_index[AVMEDIA_TYPE_SUBTITLE] =
                av_find_best_stream(mediaState->ic, AVMEDIA_TYPE_SUBTITLE,
                                    st_index[AVMEDIA_TYPE_SUBTITLE],
                                    (st_index[AVMEDIA_TYPE_AUDIO] >= 0 ?
                                     st_index[AVMEDIA_TYPE_AUDIO] :
                                     st_index[AVMEDIA_TYPE_VIDEO]),
                                    NULL, 0);

    mediaState->show_mode = SHOW_MODE_NONE;
#ifdef FFP_MERGE // bbc: dunno if we need this
    if (st_index[AVMEDIA_TYPE_VIDEO] >= 0) {
        AVStream *st = ic->streams[st_index[AVMEDIA_TYPE_VIDEO]];
        AVCodecParameters *codecpar = st->codecpar;
        AVRational sar = av_guess_sample_aspect_ratio(ic, st, NULL);
        if (codecpar->width)
            set_default_window_size(codecpar->width, codecpar->height, sar);
    }
#endif
    /* open the streams */
    if (st_index[AVMEDIA_TYPE_AUDIO] >= 0) {
        stream_component_open(mediaState, st_index[AVMEDIA_TYPE_AUDIO]);
    } else {
        mediaState->av_sync_type = AV_SYNC_VIDEO_MASTER;
    }

    int ret = -1;
    if (st_index[AVMEDIA_TYPE_VIDEO] >= 0) {
        ret = stream_component_open(mediaState, st_index[AVMEDIA_TYPE_VIDEO]);
    }
    if (mediaState->show_mode == SHOW_MODE_NONE)
        mediaState->show_mode = ret >= 0 ? SHOW_MODE_VIDEO : SHOW_MODE_RDFT;

    if (st_index[AVMEDIA_TYPE_SUBTITLE] >= 0) {
        stream_component_open(mediaState, st_index[AVMEDIA_TYPE_SUBTITLE]);
    }
    mediaState->player->msg_queue->putEmptyMessage(FFP_MSG_COMPONENT_OPEN);

    if (!mediaState->delay_inited) {
        set_avformat_context(mediaState->meta, mediaState->ic);
    }

    mediaState->stat->bit_rate = mediaState->ic->bit_rate;
    if (st_index[AVMEDIA_TYPE_VIDEO] >= 0) {
        mediaState->meta->set_int64_l(IJKM_KEY_VIDEO_STREAM, st_index[AVMEDIA_TYPE_VIDEO]);
    }
    if (st_index[AVMEDIA_TYPE_AUDIO] >= 0) {
        mediaState->meta->set_int64_l(IJKM_KEY_AUDIO_STREAM, st_index[AVMEDIA_TYPE_AUDIO]);
    }
    if (st_index[AVMEDIA_TYPE_SUBTITLE] >= 0) {
        mediaState->meta->set_int64_l(IJKM_KEY_TIMEDTEXT_STREAM, st_index[AVMEDIA_TYPE_SUBTITLE]);
    }
    if (mediaState->video_stream < 0 && mediaState->audio_stream < 0) {
        av_log(NULL, AV_LOG_FATAL, "Failed to open file '%s' or configure filtergraph\n",mediaState->filename.c_str());
        ret = -1;
    }
    if (mediaState->audio_stream >= 0) {
        mediaState->audioq->is_buffer_indicator = true;
        mediaState->buffer_indicator_queue = mediaState->audioq;
    } else if (mediaState->video_stream >= 0) {
        mediaState->videoq->is_buffer_indicator = true;
        mediaState->buffer_indicator_queue = mediaState->videoq;
    } else {
        assert("invalid streams");
    }
    if (mediaState->infinite_buffer < 0 && mediaState->realtime) {
        mediaState->infinite_buffer = 1;
    }

    if (!mediaState->start_on_prepared) {
        mediaState->player->pause(true);
    }
    if (mediaState->video_st && mediaState->video_st->codecpar) {
        AVCodecParameters *codecpar = mediaState->video_st->codecpar;
        mediaState->player->msg_queue->putEmptyMessage(FFP_MSG_VIDEO_SIZE_CHANGED, codecpar->width, codecpar->height);
        mediaState->player->msg_queue->putEmptyMessage(FFP_MSG_SAR_CHANGED, codecpar->sample_aspect_ratio.num, codecpar->sample_aspect_ratio.den);
    }
    mediaState->prepared = true;
    mediaState->player->msg_queue->putEmptyMessage(FFP_MSG_PREPARED);
    if (!mediaState->start_on_prepared) {
        while (mediaState->pause_req && !mediaState->abort_request) {
            sleep(100);
        }
    }
    if (mediaState->auto_resume) {
        mediaState->player->msg_queue->putEmptyMessage(FFP_REQ_START);
        mediaState->auto_resume = 0;
    }
    /* offset should be seeked*/
    if (mediaState->seek_at_start > 0) {
        mediaState->player->seek(mediaState->seek_at_start);
    }
}

void ReadThread::onStop() {
    XThread::onStop();
    if (mediaState->ic && !mediaState->ic)
        avformat_close_input(&mediaState->ic);

    if (!mediaState->prepared || !mediaState->abort_request) {
        mediaState->player->msg_queue->putEmptyMessage(FFP_MSG_ERROR,mediaState->error);
    }
}

AVDictionary **
ReadThread::setup_find_stream_info_opts(AVFormatContext *s, AVDictionary *codec_opts) {
    if (!s->nb_streams) {
        LOGE("setup_find_stream_info_opts nb_streams is null");
        return nullptr;
    }
    AVDictionary **opts = static_cast<AVDictionary **>(av_mallocz(
            s->nb_streams * sizeof(AVDictionary *)));
    if (!opts) {
        LOGE("setup_find_stream_info_opts opts malloc fail");
        return nullptr;
    }
    for (int i = 0; i < s->nb_streams; ++i) {
        opts[i] = filter_codec_opts(codec_opts,s->streams[i]->codecpar->codec_id,s,s->streams[i],
                                    nullptr);
    }
    return opts;
}

AVDictionary *
ReadThread::filter_codec_opts(AVDictionary *opts, enum AVCodecID codec_id,
                              AVFormatContext *s, AVStream *st, const AVCodec *codec) {
    AVDictionary    *ret = nullptr;
    AVDictionaryEntry *t = nullptr;
    int            flags = s->oformat ? AV_OPT_FLAG_ENCODING_PARAM
                                      : AV_OPT_FLAG_DECODING_PARAM;
    char          prefix = 0;
    const AVClass    *cc = avcodec_get_class();

    if (!codec)
        if (s->oformat) {
            codec = avcodec_find_encoder(codec_id);
        } else {
            codec = avcodec_find_decoder(codec_id);
        }

    switch (st->codecpar->codec_type) {
        case AVMEDIA_TYPE_VIDEO:
            prefix  = 'v';
            flags  |= AV_OPT_FLAG_VIDEO_PARAM;
            break;
        case AVMEDIA_TYPE_AUDIO:
            prefix  = 'a';
            flags  |= AV_OPT_FLAG_AUDIO_PARAM;
            break;
        case AVMEDIA_TYPE_SUBTITLE:
            prefix  = 's';
            flags  |= AV_OPT_FLAG_SUBTITLE_PARAM;
            break;
        default:
            break;
    }

    while ((t = av_dict_get(opts, "", t, AV_DICT_IGNORE_SUFFIX))) {
        char *p = strchr(t->key, ':');

        /* check stream specification in opt name */
        if (p)
            switch (check_stream_specifier(s, st, p + 1)) {
                case  1: *p = 0; break;
                case  0:         continue;
                default:         return NULL;
            }

        if (av_opt_find(&cc, t->key, NULL, flags, AV_OPT_SEARCH_FAKE_OBJ) ||
            (codec && codec->priv_class &&
             av_opt_find((void *) &codec->priv_class, t->key, NULL, flags,
                         AV_OPT_SEARCH_FAKE_OBJ)))
            av_dict_set(&ret, t->key, t->value, 0);
        else if (t->key[0] == prefix &&
                 av_opt_find(&cc, t->key + 1, NULL, flags,
                             AV_OPT_SEARCH_FAKE_OBJ))
            av_dict_set(&ret, t->key + 1, t->value, 0);

        if (p)
            *p = ':';
    }
    return ret;
}

void ReadThread::set_avformat_context(MediaMeta *meta, AVFormatContext *ic) {
    if (!meta || !ic) {
        return;
    }
    if (ic->iformat && ic->iformat->name) {
        meta->set_string_l(IJKM_KEY_FORMAT, ic->iformat->name);
    }
    if (ic->duration != AV_NOPTS_VALUE) {
        meta->set_int64_l(IJKM_KEY_DURATION_US, ic->duration);
    }

    if (ic->start_time != AV_NOPTS_VALUE) {
        meta->set_int64_l(IJKM_KEY_START_US, ic->start_time);
    }

    if (ic->bit_rate) {
        meta->set_int64_l(IJKM_KEY_BITRATE, ic->bit_rate);
    }

    MediaMeta *stream_meta = NULL;
    for (int i = 0; i < ic->nb_streams; i++) {
        if (!stream_meta) { SafeDelete(stream_meta); }

        AVStream *st = ic->streams[i];
        if (!st || !st->codecpar)
            continue;

        stream_meta = new MediaMeta();
        if (!stream_meta)
            continue;

        AVCodecParameters *codecpar = st->codecpar;
        const char *codec_name = avcodec_get_name(codecpar->codec_id);
        if (codec_name) { stream_meta->set_string_l(IJKM_KEY_CODEC_NAME, codec_name); }
        if (codecpar->profile != FF_PROFILE_UNKNOWN) {
            const AVCodec *codec = avcodec_find_decoder(codecpar->codec_id);
            if (codec) {
                stream_meta->set_int64_l(IJKM_KEY_CODEC_PROFILE_ID, codecpar->profile);
                const char *profile = av_get_profile_name(codec, codecpar->profile);
                if (profile) {
                    stream_meta->set_string_l(IJKM_KEY_CODEC_PROFILE, profile);
                }
                if (codec->long_name) {
                    stream_meta->set_string_l(IJKM_KEY_CODEC_LONG_NAME, codec->long_name);
                }
                stream_meta->set_int64_l(IJKM_KEY_CODEC_LEVEL, codecpar->level);
                if (codecpar->format != AV_PIX_FMT_NONE) {
                    stream_meta->set_string_l(IJKM_KEY_CODEC_PIXEL_FORMAT, av_get_pix_fmt_name(
                            static_cast<AVPixelFormat>(codecpar->format)));
                }
            }
        }

        int64_t bitrate = get_bit_rate(codecpar);
        if (bitrate > 0) {
            stream_meta->set_int64_l(IJKM_KEY_BITRATE, bitrate);
        }

        switch (codecpar->codec_type) {
            case AVMEDIA_TYPE_VIDEO: {
                stream_meta->set_string_l(IJKM_KEY_TYPE, IJKM_VAL_TYPE__VIDEO);

                if (codecpar->width > 0) {
                    stream_meta->set_int64_l(IJKM_KEY_WIDTH, codecpar->width);
                }
                if (codecpar->height > 0){
                    stream_meta->set_int64_l(IJKM_KEY_HEIGHT, codecpar->height);
                }
                if (st->sample_aspect_ratio.num > 0 && st->sample_aspect_ratio.den > 0) {
                    stream_meta->set_int64_l(IJKM_KEY_SAR_NUM, codecpar->sample_aspect_ratio.num);
                    stream_meta->set_int64_l(IJKM_KEY_SAR_DEN, codecpar->sample_aspect_ratio.den);
                }

                if (st->avg_frame_rate.num > 0 && st->avg_frame_rate.den > 0) {
                    stream_meta->set_int64_l(IJKM_KEY_FPS_NUM, st->avg_frame_rate.num);
                    stream_meta->set_int64_l(IJKM_KEY_FPS_DEN, st->avg_frame_rate.den);
                }
                if (st->r_frame_rate.num > 0 && st->r_frame_rate.den > 0) {
                    stream_meta->set_int64_l(IJKM_KEY_TBR_NUM, st->avg_frame_rate.num);
                    stream_meta->set_int64_l(IJKM_KEY_TBR_DEN, st->avg_frame_rate.den);
                }
                break;
            }
            case AVMEDIA_TYPE_AUDIO: {
                stream_meta->set_string_l(IJKM_KEY_TYPE, IJKM_VAL_TYPE__AUDIO);

                if (codecpar->sample_rate) {
                    stream_meta->set_int64_l(IJKM_KEY_SAMPLE_RATE, codecpar->sample_rate);
                }
                if (codecpar->channel_layout) {
                    stream_meta->set_int64_l(IJKM_KEY_CHANNEL_LAYOUT, codecpar->channel_layout);
                }
                break;
            }
            case AVMEDIA_TYPE_SUBTITLE: {
                stream_meta->set_string_l(IJKM_KEY_TYPE, IJKM_VAL_TYPE__TIMEDTEXT);
                break;
            }
            default: {
                stream_meta->set_string_l(IJKM_KEY_TYPE, IJKM_VAL_TYPE__UNKNOWN);
                break;
            }
        }

        AVDictionaryEntry *lang = av_dict_get(st->metadata, "language", NULL, 0);
        if (lang && lang->value) {
            stream_meta->set_string_l(IJKM_KEY_LANGUAGE, lang->value);
        }
        meta->appendChild(stream_meta);
        SafeDelete(stream_meta);
    }


}
