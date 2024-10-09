//
// Created by Administrator on 2024/9/23.
//

#include "VideoThread.h"


VideoThread::VideoThread(VideoState *is,int stream_index,int stream_lowers)
        :StreamThread(is,stream_index,stream_lowers)
{
    std::ostringstream log;
    frame = av_frame_alloc();
    if (!frame) {
        log<<"frame is null";
        LOGE("%s",log.str().c_str());
    }
    if (stream_index <0 || stream_index>= mediaState->ic->nb_streams) {
        log<<"stream_index error:"<<stream_index;
        LOGE("%s",log.str().c_str());
    }
    avctx = avcodec_alloc_context3(NULL);
    if (!avctx) {
        log<<"avctx error:"<< AVERROR(ENOMEM);
        LOGE("%s",log.str().c_str());
    }
    err_code = avcodec_parameters_to_context(avctx,mediaState->ic->streams[stream_index]->codecpar);
    if (err_code <0) {
        log<<"avcodec_parameters_to_context error";
        LOGE("%s",log.str().c_str());
    }
    avctx->pkt_timebase = mediaState->ic->streams[stream_index]->time_base;
    codec = avcodec_find_decoder(avctx->codec_id);

    mediaState->last_video_stream = stream_index;
    forced_codec_name = mediaState->video_codec_name;

    if (!forced_codec_name.empty()) {
        codec = avcodec_find_decoder_by_name(forced_codec_name.c_str());
    }
    if (!codec) {
        if (forced_codec_name.empty()) {
            log<<"No decoder could be found for codec "<<avcodec_get_name(avctx->codec_id);
        }else {
            log<<"No codec could be found with name "<<forced_codec_name;
        }
        log<< AVERROR(EINVAL);
        LOGE("%s",log.str().c_str());
    }
    avctx->codec_id = codec->id;
    if (stream_lowers > codec->max_lowres) {
        log<<"The maximum value for lowres supported by the decoder is "<<codec->max_lowres<<"\n";
        stream_lowers = codec->max_lowres;
    }
    avctx->lowres = stream_lowers;
    if (mediaState->fast) {
        avctx->flags2 |= AV_CODEC_FLAG2_FAST;
    }
    err_code = filter_codec_opts(mediaState->codec_opts, avctx->codec_id, mediaState->ic,
                                 mediaState->ic->streams[stream_index], codec, &opts);
    if (err_code < 0) {
        log<<"filter_codec_opts error";
        LOGE("%s",log.str().c_str());
    }
    if (!av_dict_get(opts, "threads", nullptr, 0))

        av_dict_set(&opts, "threads", "auto", 0);
    if (stream_lowers)
        av_dict_set_int(&opts, "lowres", stream_lowers, 0);

    av_dict_set(&opts, "flags", "+copy_opaque", AV_DICT_MULTIKEY);

    err_code = create_hwaccel(&avctx->hw_device_ctx);
    if (err_code < 0) {
        log<<"create_hwaccel error";
        LOGE("%s",log.str().c_str());
    }

    if ((err_code = avcodec_open2(avctx, codec, &opts)) < 0) {
        log<<"avcodec_open2 error";
        LOGE("%s",log.str().c_str());
    }
    if ((t = av_dict_get(opts, "", NULL, AV_DICT_IGNORE_SUFFIX))) {
        av_log(NULL, AV_LOG_ERROR, "Option %s not found.\n", t->key);
        err_code =  AVERROR_OPTION_NOT_FOUND;
        log<<"AVERROR_OPTION_NOT_FOUND error";
        LOGE("%s",log.str().c_str());
    }


    mediaState->eof = 0;
    mediaState->ic->streams[stream_index]->discard = AVDISCARD_DEFAULT;
    mediaState->video_stream = stream_index;
    mediaState->video_st = mediaState->ic->streams[stream_index];
    mediaState->viddec = new Decoder(avctx,mediaState->videoq);
    LOGD("%s",log.str().c_str());
}

VideoThread::~VideoThread() {

}

void VideoThread::onCreate() {


}

void VideoThread::onThreadRun(uint32_t now) {
    std::ostringstream log;
    int ret = get_video_frame();
    if (ret <0) {
        log<<"get_video_frame return err";
        LOGE("%s",log.str().c_str());
        stopThread();
    }
    if (!ret) {
        return;
    }
    if (last_w != frame->width
    || last_h != frame->height
           || last_format != frame->format
           || last_serial != mediaState->viddec->pkt_serial
           || last_vfilter_idx != mediaState->vfilter_idx) {
        av_log(NULL, AV_LOG_DEBUG,
               "Video frame changed from size:%dx%d format:%s serial:%d to size:%dx%d format:%s serial:%d\n",
               last_w, last_h,
               (const char *)av_x_if_null(av_get_pix_fmt_name(last_format), "none"), last_serial,
               frame->width, frame->height,
               (const char *)av_x_if_null(av_get_pix_fmt_name(
                       static_cast<AVPixelFormat>(frame->format)), "none"), mediaState->viddec->pkt_serial);
        avfilter_graph_free(&graph);
        graph = avfilter_graph_alloc();
        if (!graph) {
            ret = AVERROR(ENOMEM);
            log<<"avfilter_graph_alloc return err";
            LOGE("%s",log.str().c_str());
        }
        graph->nb_threads = mediaState->filter_nbthreads;
        if ((ret = configure_video_filters(graph, mediaState, mediaState->vfilters_list ? mediaState->vfilters_list[mediaState->vfilter_idx] : NULL, frame)) < 0) {
            SDL_Event event;
            event.type = FF_QUIT_EVENT;
            event.user.data1 = is;
            SDL_PushEvent(&event);
            goto the_end;
        }
        filt_in  = is->in_video_filter;
        filt_out = is->out_video_filter;
        last_w = frame->width;
        last_h = frame->height;
        last_format = frame->format;
        last_serial = is->viddec.pkt_serial;
        last_vfilter_idx = is->vfilter_idx;
        frame_rate = av_buffersink_get_frame_rate(filt_out);
    }


}

void VideoThread::onStop() {
    mediaState->queue_attachments_req = 1;
}

int VideoThread::create_hwaccel(AVBufferRef **device_ctx) {
    enum AVHWDeviceType type;
    int ret;
    AVBufferRef *vk_dev;

    *device_ctx = NULL;

    if (!mediaState->hwaccel)
        return 0;

    type = av_hwdevice_find_type_by_name(mediaState->hwaccel);
    if (type == AV_HWDEVICE_TYPE_NONE)
        return AVERROR(ENOTSUP);

    //use vk renderer???
//    ret = vk_renderer_get_hw_dev(vk_renderer, &vk_dev);
//    if (ret < 0)
//        return ret;

    ret = av_hwdevice_ctx_create_derived(device_ctx, type, vk_dev, 0);
    if (!ret)
        return 0;

    if (ret != AVERROR(ENOSYS))
        return ret;

    av_log(NULL, AV_LOG_WARNING, "Derive %s from vulkan not supported.\n", mediaState->hwaccel);
    ret = av_hwdevice_ctx_create(device_ctx, type, NULL, NULL, 0);
    return ret;
}

int VideoThread::get_video_frame() {
    int got_picture;
    if ((got_picture = decoder_decode_frame()) <0) {
        return -1;
    }
    if (got_picture) {
        double dpts = NAN;

        if (frame->pts != AV_NOPTS_VALUE) {
            dpts = av_q2d(mediaState->video_st->time_base)*frame->pts;
        }
        frame->sample_aspect_ratio = av_guess_sample_aspect_ratio(mediaState->ic,mediaState->video_st,frame);

        if (framedrop>0 || (framedrop && get_master_sync_type() != AV_SYNC_VIDEO_MASTER)) {
            if (frame->pts != AV_NOPTS_VALUE) {
                double diff = dpts - get_master_clock();
                if (!isnan(diff) && fabs(diff) < AV_NOSYNC_THRESHOLD &&
                    diff - mediaState->frame_last_filter_delay < 0 &&
                        mediaState->viddec->pkt_serial == mediaState->vidclk->serial &&
                        mediaState->videoq->nb_packets) {
                    mediaState->frame_drops_early++;
                    av_frame_unref(frame);
                    got_picture = 0;
                }
            }
        }
    }
    return got_picture;
}

int VideoThread::decoder_decode_frame() {
    int ret = AVERROR(EAGAIN);
    Decoder *decoder = mediaState->viddec;
    while (true) {
        if (decoder->queue->serial == decoder->pkt_serial) {
            do {
                if (decoder->queue->abort_request) {
                    return -1;
                }
                //AVMEDIA_TYPE_VIDEO
                ret = avcodec_receive_frame(decoder->avctx, frame);
                if (ret <= 0) {
                    if (mediaState->decoder_reorder_pts == -1) {
                        frame->pts = frame->best_effort_timestamp;
                    } else if (!mediaState->decoder_reorder_pts) {
                        frame->pts = frame->pkt_dts;
                    }
                }

                if (ret == AVERROR_EOF) {
                    decoder->finished = decoder->pkt_serial;
                    avcodec_flush_buffers(decoder->avctx);
                    return 0;
                }
                if (ret >= 0) {
                    return 1;
                }
            } while (ret != AVERROR(EAGAIN));
        }
        do {
            if (decoder->queue->nb_packets == 0) {
//                        SDL_CondSignal(d->empty_queue_cond);
            }
            if (decoder->packet_pending) {
                decoder->packet_pending = 0;
            } else {
                int old_serial = decoder->pkt_serial;
                if (decoder->queue->get(1, &decoder->pkt_serial)) {
                    return -1;
                }
                if (old_serial != decoder->pkt_serial) {
                    avcodec_flush_buffers(decoder->avctx);
                    decoder->finished = 0;
                    decoder->next_pts = decoder->start_pts;
                }
            }
            if (decoder->queue->serial == decoder->pkt_serial) {
                break;
            }
            av_packet_unref(decoder->pkt);
        } while (true);


//                    //??????????
//                    if (decoder->pkt->buf && !decoder->pkt->opaque_ref) {
//
//                        decoder->pkt->opaque_ref = av_buffer_allocz(sizeof(int64_t*));
//                        if (!decoder->pkt->opaque_ref)
//                            return AVERROR(ENOMEM);
//                        decoder->pkt->opaque_ref->data = (int64_t)decoder->pkt->pos;
//                    }

        if (avcodec_send_packet(decoder->avctx, decoder->pkt) == AVERROR(EAGAIN)) {
            av_log(decoder->avctx, AV_LOG_ERROR,
                   "Receive_frame and send_packet both returned EAGAIN, which is an API violation.\n");
            decoder->packet_pending = 1;
        } else {
            av_packet_unref(decoder->pkt);
        }
    }
}

int VideoThread::configure_video_filters() {
//    enum AVPixelFormat pix_fmts[FF_ARRAY_ELEMS(sdl_texture_format_map)];
    char sws_flags_str[512] = "";
    char buffersrc_args[256];
    int ret;
    AVFilterContext *filt_src = NULL, *filt_out = NULL, *last_filter = NULL;
    AVCodecParameters *codecpar = mediaState->video_st->codecpar;
    AVRational fr = av_guess_frame_rate(mediaState->ic, mediaState->video_st, NULL);
    const AVDictionaryEntry *e = NULL;
    int nb_pix_fmts = 0;
    int i, j;
    AVBufferSrcParameters *par = av_buffersrc_parameters_alloc();

    if (!par)
        return AVERROR(ENOMEM);

//    for (i = 0; i < renderer_info.num_texture_formats; i++) {
//        for (j = 0; j < FF_ARRAY_ELEMS(sdl_texture_format_map) - 1; j++) {
//            if (renderer_info.texture_formats[i] == sdl_texture_format_map[j].texture_fmt) {
//                pix_fmts[nb_pix_fmts++] = sdl_texture_format_map[j].format;
//                break;
//            }
//        }
//    }
//    pix_fmts[nb_pix_fmts] = AV_PIX_FMT_NONE;

    while ((e = av_dict_iterate(sws_dict, e))) {
        if (!strcmp(e->key, "sws_flags")) {
            av_strlcatf(sws_flags_str, sizeof(sws_flags_str), "%s=%s:", "flags", e->value);
        } else
            av_strlcatf(sws_flags_str, sizeof(sws_flags_str), "%s=%s:", e->key, e->value);
    }
    if (strlen(sws_flags_str))
        sws_flags_str[strlen(sws_flags_str)-1] = '\0';

    graph->scale_sws_opts = av_strdup(sws_flags_str);

    snprintf(buffersrc_args, sizeof(buffersrc_args),
             "video_size=%dx%d:pix_fmt=%d:time_base=%d/%d:pixel_aspect=%d/%d:"
             "colorspace=%d:range=%d",
             frame->width, frame->height, frame->format,
             mediaState->video_st->time_base.num, mediaState->video_st->time_base.den,
             codecpar->sample_aspect_ratio.num, FFMAX(codecpar->sample_aspect_ratio.den, 1),
             frame->colorspace, frame->color_range);
    if (fr.num && fr.den)
        av_strlcatf(buffersrc_args, sizeof(buffersrc_args), ":frame_rate=%d/%d", fr.num, fr.den);

    if ((ret = avfilter_graph_create_filter(&filt_src,
                                            avfilter_get_by_name("buffer"),
                                            "ffplay_buffer", buffersrc_args, NULL,
                                            graph)) < 0)
        goto fail;
    par->hw_frames_ctx = frame->hw_frames_ctx;
    ret = av_buffersrc_parameters_set(filt_src, par);
    if (ret < 0)
        goto fail;

    ret = avfilter_graph_create_filter(&filt_out,
                                       avfilter_get_by_name("buffersink"),
                                       "ffplay_buffersink", NULL, NULL, graph);
    if (ret < 0)
        goto fail;

    if ((ret = av_opt_set_int_list(filt_out, "pix_fmts", pix_fmts,  AV_PIX_FMT_NONE, AV_OPT_SEARCH_CHILDREN)) < 0)
        goto fail;
    if (!vk_renderer &&
        (ret = av_opt_set_int_list(filt_out, "color_spaces", sdl_supported_color_spaces,  AVCOL_SPC_UNSPECIFIED, AV_OPT_SEARCH_CHILDREN)) < 0)
        goto fail;

    last_filter = filt_out;

/* Note: this macro adds a filter before the lastly added filter, so the
 * processing order of the filters is in reverse */
#define INSERT_FILT(name, arg) do {                                          \
    AVFilterContext *filt_ctx;                                               \
                                                                             \
    ret = avfilter_graph_create_filter(&filt_ctx,                            \
                                       avfilter_get_by_name(name),           \
                                       "ffplay_" name, arg, NULL, graph);    \
    if (ret < 0)                                                             \
        goto fail;                                                           \
                                                                             \
    ret = avfilter_link(filt_ctx, 0, last_filter, 0);                        \
    if (ret < 0)                                                             \
        goto fail;                                                           \
                                                                             \
    last_filter = filt_ctx;                                                  \
} while (0)

    if (mediaState->autorotate) {
        double theta = 0.0;
        int32_t *displaymatrix = NULL;
        AVFrameSideData *sd = av_frame_get_side_data(frame, AV_FRAME_DATA_DISPLAYMATRIX);
        if (sd)
            displaymatrix = (int32_t *)sd->data;
        if (!displaymatrix) {
            const AVPacketSideData *psd = av_packet_side_data_get(is->video_st->codecpar->coded_side_data,
                                                                  is->video_st->codecpar->nb_coded_side_data,
                                                                  AV_PKT_DATA_DISPLAYMATRIX);
            if (psd)
                displaymatrix = (int32_t *)psd->data;
        }
        theta = get_rotation(displaymatrix);

        if (fabs(theta - 90) < 1.0) {
            INSERT_FILT("transpose", "clock");
        } else if (fabs(theta - 180) < 1.0) {
            INSERT_FILT("hflip", NULL);
            INSERT_FILT("vflip", NULL);
        } else if (fabs(theta - 270) < 1.0) {
            INSERT_FILT("transpose", "cclock");
        } else if (fabs(theta) > 1.0) {
            char rotate_buf[64];
            snprintf(rotate_buf, sizeof(rotate_buf), "%f*PI/180", theta);
            INSERT_FILT("rotate", rotate_buf);
        }
    }

    if ((ret = configure_filtergraph(graph, vfilters, filt_src, last_filter)) < 0)
        goto fail;

    mediaState->in_video_filter  = filt_src;
    mediaState->out_video_filter = filt_out;

    fail:
    av_freep(&par);
    return ret;
}
