//
// Created by PeterXi on 2024/8/10.
//

#include "ReadThread.h"

ReadThread::ReadThread(VideoState *is)
        : XThread("ReadThread", 10, URGENT)
        ,mediaState(is){

}

ReadThread::~ReadThread() {

}

void ReadThread::onThreadRun(uint32_t now) {



}

void ReadThread::onCreate() {
    XThread::onCreate();
    mediaState->last_video_stream = mediaState->video_stream = -1;
    mediaState->last_audio_stream = mediaState->audio_stream = -1;
    mediaState->last_subtitle_stream = mediaState->subtitle_stream = -1;
    mediaState->eof = 0;

    mediaState->ic = avformat_alloc_context();
    if (!mediaState->ic) {
        LOGE("ReadThread::onCreate ERROR");
    }
    mediaState->ic->interrupt_callback.callback = mediaState->decode_interrupt_cb;
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
        mediaState->meta->set_int64_l(IJKM_KEY_TIMEDTEXT_STREAM, st_index[AVMEDIA_TYPE_SUBTITLE])
    }
    if (is->video_stream < 0 && is->audio_stream < 0) {
        av_log(NULL, AV_LOG_FATAL, "Failed to open file '%s' or configure filtergraph\n",mediaState->filename.c_str());
        ret = -1;
    }
//    if (is->audio_stream >= 0) {
//        is->audioq.is_buffer_indicator = 1;
//        is->buffer_indicator_queue = &is->audioq;
//    } else if (is->video_stream >= 0) {
//        is->videoq.is_buffer_indicator = 1;
//        is->buffer_indicator_queue = &is->videoq;
//    } else {
//        assert("invalid streams");
//    }
//
//    if (ffp->infinite_buffer < 0 && is->realtime)
//        ffp->infinite_buffer = 1;
//
//    if (!ffp->render_wait_start && !ffp->start_on_prepared)
//        toggle_pause(ffp, 1);
//    if (is->video_st && is->video_st->codecpar) {
//        AVCodecParameters *codecpar = is->video_st->codecpar;
//        ffp_notify_msg3(ffp, FFP_MSG_VIDEO_SIZE_CHANGED, codecpar->width, codecpar->height);
//        ffp_notify_msg3(ffp, FFP_MSG_SAR_CHANGED, codecpar->sample_aspect_ratio.num, codecpar->sample_aspect_ratio.den);
//    }
//    ffp->prepared = true;
//    ffp_notify_msg1(ffp, FFP_MSG_PREPARED);
//    if (!ffp->render_wait_start && !ffp->start_on_prepared) {
//        while (is->pause_req && !is->abort_request) {
//            SDL_Delay(20);
//        }
//    }
//    if (ffp->auto_resume) {
//        ffp_notify_msg1(ffp, FFP_REQ_START);
//        ffp->auto_resume = 0;
//    }
//    /* offset should be seeked*/
//    if (ffp->seek_at_start > 0) {
//        ffp_seek_to_l(ffp, (long)(ffp->seek_at_start));
//    }

}

void ReadThread::onStop() {
    XThread::onStop();
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
