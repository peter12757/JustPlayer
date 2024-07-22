//
// Created by Peter Xi on 2022/6/3.
//

#include "LocalDataReader.h"


LocalDataReader::LocalDataReader(MediaState *vs)
        : XThread("LocalDataReader", 100, URGENT), is(vs) {
    std::ostringstream logOs;
    logOs << "LocalDataReader::LocalDataReader";
    pkt = av_packet_alloc();
    if (!pkt) {
        logOs << "Could not allocate packet.\n";
    }
    ic = avformat_alloc_context();
    if (!ic) {
        logOs << "Could not allocate context.\n";
    }
    ic->interrupt_callback.callback = &LocalDataReader::decode_interrupt_cb;
    ic->interrupt_callback.opaque = is;
    LOGD("%s", logOs.str().c_str());
}

LocalDataReader::~LocalDataReader() {
    if (ic && !is->ic)
        avformat_close_input(&ic);
    av_packet_free(&pkt);

}

void LocalDataReader::onCreate() {
    std::ostringstream logOs;
    logOs << "LocalDataReader::onCreate";
    int err, i, ret;

    memset(st_index, -1, sizeof(st_index));
    is->eof = 0;

    if (!av_dict_get(format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE))
        if (!av_dict_get(format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE)) {
            av_dict_set(&format_opts, "scan_all_pmts", "1", AV_DICT_DONT_OVERWRITE);
            scan_all_pmts_set = 1;
        }
    err = avformat_open_input(&ic, is->filename->c_str(), is->iformat, &format_opts);
    if (err < 0) {
        logOs << AVERROR(err);
    }
    if (scan_all_pmts_set)
        av_dict_set(&format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE);

    if ((t = av_dict_get(format_opts, "", NULL, AV_DICT_IGNORE_SUFFIX))) {
        logOs << AVERROR(AVERROR_OPTION_NOT_FOUND) << "Option " << t->key << " not found.";
    }
    is->ic = ic;

    if (FFConfig::Instant()->genpts)
        ic->flags |= AVFMT_FLAG_GENPTS;

    av_format_inject_global_side_data(ic);

    //find stream options find_stream_info
    AVDictionary **opts;

    if (!ic->nb_streams)
        logOs << "nb_streams is null";
    opts = static_cast<AVDictionary **>(av_calloc(ic->nb_streams, sizeof(*opts)));
    if (!opts) {
        logOs << "Could not alloc memory for stream options.\n";
    }
    for (i = 0; i < ic->nb_streams; i++)
        opts[i] = filter_codec_opts(codec_opts, ic->streams[i]->codecpar->codec_id,
                                    ic, ic->streams[i], NULL);

    int orig_nb_streams = ic->nb_streams;

    err = avformat_find_stream_info(ic, opts);

    for (i = 0; i < orig_nb_streams; i++)
        av_dict_free(&opts[i]);
    av_freep(&opts);

    if (err < 0) {
        logOs << is->filename << " could not find codec parameters\n";
    }
    //find_stream_info end

    if (ic->pb)
        ic->pb->eof_reached = 0; // FIXME hack, ffplay maybe should not use avio_feof() to test for the end

    if (FFConfig::Instant()->seek_by_bytes < 0)
        FFConfig::Instant()->seek_by_bytes =
                !!(ic->iformat->flags & AVFMT_TS_DISCONT) && strcmp("ogg", ic->iformat->name);

    is->m_videoObj->max_frame_duration = (ic->iformat->flags & AVFMT_TS_DISCONT) ? 10.0 : 3600.0;


    if (!ctx->m_media_title && (t = av_dict_get(ic->metadata, "title", NULL, 0)))
        ctx->m_media_title = new string(av_asprintf("%s - %s", t->value, is->filename));

    /* if seeking requested, we execute it */
    if (start_time != AV_NOPTS_VALUE) {
        int64_t timestamp;

        timestamp = start_time;
        /* add the stream start time */
        if (ic->start_time != AV_NOPTS_VALUE)
            timestamp += ic->start_time;
        ret = avformat_seek_file(ic, -1, INT64_MIN, timestamp, INT64_MAX, 0);
        if (ret < 0) {
            logOs << is->filename << " could not seek to position "
                  << (double) timestamp / AV_TIME_BASE;
        }
    }

    //is_realtime
    is->realtime = 0;
    if (!strcmp(is->iformat->name, "rtp")
        || !strcmp(is->iformat->name, "rtsp")
        || !strcmp(is->iformat->name, "sdp")
            )
        is->realtime = 1;

    if (ic->pb && (!strncmp(ic->url, "rtp:", 4)
                   || !strncmp(ic->url, "udp:", 4)
    ))
        is->realtime = 1;
    //is_realtime end

    if (FFConfig::Instant()->dump_format)
        av_dump_format(ic, 0, is->filename->c_str(), 0);

    for (i = 0; i < ic->nb_streams; i++) {
        AVStream *st = ic->streams[i];
        enum AVMediaType type = st->codecpar->codec_type;
        st->discard = AVDISCARD_ALL;
//        if (playerType >= 0 && wanted_stream_spec[playerType] && st_index[playerType] == -1)
//            if (avformat_match_stream_specifier(ic, st, wanted_stream_spec[playerType]) > 0)
//                st_index[playerType] = i;
    }
//    for (i = 0; i < AVMEDIA_TYPE_NB; i++) {
//        if (wanted_stream_spec[i] && st_index[i] == -1) {
//            av_log(NULL, AV_LOG_ERROR, "Stream specifier %s does not match any %s stream\n", wanted_stream_spec[i], av_get_media_type_string(i));
//            st_index[i] = INT_MAX;
//        }
//    }

    st_index[AVMEDIA_TYPE_VIDEO] =
            av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO,
                                st_index[AVMEDIA_TYPE_VIDEO], -1, NULL, 0);
    st_index[AVMEDIA_TYPE_AUDIO] =
            av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO,
                                st_index[AVMEDIA_TYPE_AUDIO],
                                st_index[AVMEDIA_TYPE_VIDEO],
                                NULL, 0);
    st_index[AVMEDIA_TYPE_SUBTITLE] =
            av_find_best_stream(ic, AVMEDIA_TYPE_SUBTITLE,
                                st_index[AVMEDIA_TYPE_SUBTITLE],
                                (st_index[AVMEDIA_TYPE_AUDIO] >= 0 ?
                                 st_index[AVMEDIA_TYPE_AUDIO] :
                                 st_index[AVMEDIA_TYPE_VIDEO]),
                                NULL, 0);

    //set show mode
    if (st_index[AVMEDIA_TYPE_VIDEO] >= 0) {
        AVStream *st = ic->streams[st_index[AVMEDIA_TYPE_VIDEO]];
        AVCodecParameters *codecpar = st->codecpar;
        AVRational sar = av_guess_sample_aspect_ratio(ic, st, NULL);
//        if (codecpar->width)
//            set_default_window_size(codecpar->width, codecpar->height, sar);
    }
    //set show mode end

    /* open the streams */
    if (st_index[AVMEDIA_TYPE_AUDIO] >= 0) {
        ctx->stream_component_open(st_index[AVMEDIA_TYPE_AUDIO], codec_opts);
    }

    ret = -1;
    if (st_index[AVMEDIA_TYPE_VIDEO] >= 0) {
        ret = ctx->stream_component_open(st_index[AVMEDIA_TYPE_VIDEO], codec_opts);
    }

    if (st_index[AVMEDIA_TYPE_SUBTITLE] >= 0) {
        ctx->stream_component_open(st_index[AVMEDIA_TYPE_SUBTITLE], codec_opts);
    }

    if (is->m_videoObj->video_stream < 0 && is->m_audioObj->audio_stream < 0) {
        av_log(NULL, AV_LOG_FATAL, "Failed to open file '%s' or configure filtergraph\n",
               is->filename);
        ret = -1;
    }

    if (infinite_buffer < 0 && is->realtime)
        infinite_buffer = 1;
}

void LocalDataReader::onStop() {
    XThread::onStop();

//        SDL_Event event;
//        event.playerType = FF_QUIT_EVENT;
//        event.user.data1 = is;
//        SDL_PushEvent(&event);
//    SDL_DestroyMutex(wait_mutex);
}


void LocalDataReader::packet_queue_flush(PacketQueue *q) {
    MyAVPacketList pkt1;

//    SDL_LockMutex(q->mutex);
    while (av_fifo_size(q->pkt_list) >= sizeof(pkt1)) {
        av_fifo_generic_read(q->pkt_list, &pkt1, sizeof(pkt1), NULL);
        av_packet_free(&pkt1.pkt);
    }
    q->nb_packets = 0;
    q->size = 0;
    q->duration = 0;
    q->serial++;
//    SDL_UnlockMutex(q->mutex);
}

void LocalDataReader::start() {
    startThread();
}

void LocalDataReader::onThreadRun(uint32_t now) {
    int ret = 0;
    if (is->abort_request) {
        stopThread();
        return;
    }
    if (is->paused != is->last_paused) {
        is->last_paused = is->paused;
        if (is->paused)
            is->read_pause_return = av_read_pause(ic);
        else
            av_read_play(ic);
    }
    if (is->paused &&
        (!strcmp(ic->iformat->name, "rtsp") ||
         (ic->pb && !strncmp(is->filename->c_str(), "mmsh:", 5)))) {
        /* wait 10 ms to avoid trying to get another packet */
        /* XXX: horrible */
        sleep(10);
        return;
    }
    if (is->seek_req) {
        int64_t seek_target = is->seek_pos;
        int64_t seek_min = is->seek_rel > 0 ? seek_target - is->seek_rel + 2 : INT64_MIN;
        int64_t seek_max = is->seek_rel < 0 ? seek_target - is->seek_rel - 2 : INT64_MAX;
// FIXME the +-2 is due to rounding being not done in the correct direction in generation
//      of the seek_pos/seek_rel variables

        ret = avformat_seek_file(is->ic, -1, seek_min, seek_target, seek_max, is->seek_flags);
        if (ret < 0) {
            av_log(NULL, AV_LOG_ERROR,
                   "%s: error while seeking\n", is->ic->url);
        } else {
            if (is->m_audioObj->audio_stream >= 0)
                packet_queue_flush(is->m_audioObj->audioq);
            if (is->m_subtitleObj->subtitle_stream >= 0)
                packet_queue_flush(is->m_subtitleObj->subtitleq);
            if (is->m_videoObj->video_stream >= 0)
                packet_queue_flush(is->m_videoObj->videoq);
            if (is->seek_flags & AVSEEK_FLAG_BYTE) {
                is->m_subtitleObj->extclk->set_clock(NAN, 0);
            } else {
                is->m_subtitleObj->extclk->set_clock(seek_target / (double) AV_TIME_BASE, 0);
            }
        }
        is->seek_req = 0;
        is->queue_attachments_req = 1;
        is->eof = 0;
        if (is->paused)
            is->step_to_next_frame();
    }
    if (is->queue_attachments_req) {
        if (is->m_videoObj->video_st &&
            is->m_videoObj->video_st->disposition & AV_DISPOSITION_ATTACHED_PIC) {
            if ((ret = av_packet_ref(pkt, &is->m_videoObj->video_st->attached_pic)) < 0)
//                goto fail;
                return;
            is->m_videoObj->videoq->packet_queue_put(pkt);
            is->m_videoObj->videoq->packet_queue_put_nullpacket(pkt, is->m_videoObj->video_stream);
        }
        is->queue_attachments_req = 0;
    }

    /* if the queue are full, no need to read more */
    if (infinite_buffer < 1 &&
        (is->m_audioObj->audioq->size + is->m_videoObj->videoq->size +
         is->m_subtitleObj->subtitleq->size > MAX_QUEUE_SIZE
         || (is->m_audioObj->audioq->stream_has_enough_packets(is->m_audioObj->audio_st,
                                                               is->m_audioObj->audio_stream) &&
             is->m_videoObj->videoq->stream_has_enough_packets(is->m_videoObj->video_st,
                                                               is->m_videoObj->video_stream) &&
             is->m_subtitleObj->subtitleq->stream_has_enough_packets(is->m_subtitleObj->subtitle_st,
                                                                     is->m_subtitleObj->subtitle_stream)))) {
        /* wait 10 ms */
//        SDL_LockMutex(wait_mutex);
//        SDL_CondWaitTimeout(is->continue_read_thread, wait_mutex, 10);
//        SDL_UnlockMutex(wait_mutex);
//        continue;
        return;
    }
    if (!is->paused &&
        (!is->m_audioObj->audio_st || (is->auddec->finished == is->m_audioObj->audioq->serial &&
                                       is->m_audioObj->sampq->frame_queue_nb_remaining() == 0)) &&
        (!is->m_videoObj->video_st || (is->viddec->finished == is->m_videoObj->videoq->serial &&
                                       is->m_videoObj->pictq->frame_queue_nb_remaining() == 0))) {
        if (FFConfig::Instant()->loop != 1 && (!FFConfig::Instant()->loop
                                               || --FFConfig::Instant()->loop)) {
            is->stream_seek(start_time != AV_NOPTS_VALUE ? start_time : 0, 0, 0);
        } else if (FFConfig::Instant()->autoexit) {
            ret = AVERROR_EOF;
//            goto fail;
            stopThread();
        }
    }
    ret = av_read_frame(ic, pkt);
    if (ret < 0) {
        if ((ret == AVERROR_EOF || avio_feof(ic->pb)) && !is->eof) {
            if (is->m_videoObj->video_stream >= 0)
                is->m_videoObj->videoq->packet_queue_put_nullpacket(pkt,
                                                                    is->m_videoObj->video_stream);
            if (is->m_audioObj->audio_stream >= 0)
                is->m_audioObj->audioq->packet_queue_put_nullpacket(pkt,
                                                                    is->m_audioObj->audio_stream);
            if (is->m_subtitleObj->subtitle_stream >= 0)
                is->m_subtitleObj->subtitleq->packet_queue_put_nullpacket(pkt,
                                                                          is->m_subtitleObj->subtitle_stream);
            is->eof = 1;
        }
        if (ic->pb && ic->pb->error) {
            if (FFConfig::Instant()->autoexit)
//                goto fail;
                return;
            else
//                break;
                return;
        }
//        SDL_LockMutex(wait_mutex);
//        SDL_CondWaitTimeout(is->continue_read_thread, wait_mutex, 10);
//        SDL_UnlockMutex(wait_mutex);
//        continue;
        return;
    } else {
        is->eof = 0;
    }
    /* check if packet is in play range specified by user, then queue, otherwise discard */
    stream_start_time = ic->streams[pkt->stream_index]->start_time;
    pkt_ts = pkt->pts == AV_NOPTS_VALUE ? pkt->dts : pkt->pts;
    pkt_in_play_range = duration == AV_NOPTS_VALUE ||
                        (pkt_ts - (stream_start_time != AV_NOPTS_VALUE ? stream_start_time : 0)) *
                        av_q2d(ic->streams[pkt->stream_index]->time_base) -
                        (double) (start_time != AV_NOPTS_VALUE ? start_time : 0) / 1000000
                        <= ((double) duration / 1000000);
    if (pkt->stream_index == is->m_audioObj->audio_stream && pkt_in_play_range) {
        is->m_audioObj->audioq->packet_queue_put(pkt);
    } else if (pkt->stream_index == is->m_videoObj->video_stream && pkt_in_play_range
               && !(is->m_videoObj->video_st->disposition & AV_DISPOSITION_ATTACHED_PIC)) {
        is->m_videoObj->videoq->packet_queue_put(pkt);
    } else if (pkt->stream_index == is->m_subtitleObj->subtitle_stream && pkt_in_play_range) {
        is->m_subtitleObj->subtitleq->packet_queue_put(pkt);
    } else {
        av_packet_unref(pkt);
    }
}

int LocalDataReader::decode_interrupt_cb(void *ctx) {
    std::ostringstream logOs;
    LOGD("%s",logOs.str().c_str());
}

AVDictionary *
LocalDataReader::filter_codec_opts(AVDictionary *opts, enum AVCodecID codec_id, AVFormatContext *s,
                                   AVStream *st, const AVCodec *codec) {
    AVDictionary *ret = NULL;
    const AVDictionaryEntry *t = NULL;
    int flags = s->oformat ? AV_OPT_FLAG_ENCODING_PARAM
                           : AV_OPT_FLAG_DECODING_PARAM;
    char prefix = 0;
    const AVClass *cc = avcodec_get_class();

    if (!codec)
        codec = s->oformat ? avcodec_find_encoder(codec_id)
                           : avcodec_find_decoder(codec_id);

    switch (st->codecpar->codec_type) {
        case AVMEDIA_TYPE_VIDEO:
            prefix = 'v';
            flags |= AV_OPT_FLAG_VIDEO_PARAM;
            break;
        case AVMEDIA_TYPE_AUDIO:
            prefix = 'a';
            flags |= AV_OPT_FLAG_AUDIO_PARAM;
            break;
        case AVMEDIA_TYPE_SUBTITLE:
            prefix = 's';
            flags |= AV_OPT_FLAG_SUBTITLE_PARAM;
            break;
    }

    while (t = av_dict_get(opts, "", t, AV_DICT_IGNORE_SUFFIX)) {
        const AVClass *priv_class;
        char *p = strchr(t->key, ':');

        /* check stream specification in opt name */
        if (p)
            switch (avformat_match_stream_specifier(s, st, p + 1)) {
                case 1:
                    *p = 0;
                    break;
                case 0:
                    continue;
                default:
                    LOGD("avformat_match_stream_specifier ERR");
            }

        if (av_opt_find(&cc, t->key, NULL, flags, AV_OPT_SEARCH_FAKE_OBJ) ||
            !codec ||
            ((priv_class = codec->priv_class) &&
             av_opt_find(&priv_class, t->key, NULL, flags,
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