//
// Created by Peter Xi on 2022/6/5.
//

#include "AudioDecodec.h"


AudioDecodec::AudioDecodec(MediaState *vs)
        : XThread("AudioDecodec",100,URGENT)
        ,is(vs) {
    frame = av_frame_alloc();

}


AudioDecodec::~AudioDecodec() {
    avfilter_graph_free(&is->agraph);
    av_frame_free(&frame);

}

int AudioDecodec::decoder_decode_frame(Decoder *d, AVSubtitle *sub) {
    int ret = AVERROR(EAGAIN);
    for (;;) {
        if (d->queue->serial == d->pkt_serial) {
            do {
                if (d->queue->abort_request)
                    return -1;

                switch (d->avctx->codec_type) {
                    case AVMEDIA_TYPE_VIDEO:
                        ret = avcodec_receive_frame(d->avctx, frame);
                        if (ret >= 0) {
                            if (decoder_reorder_pts == -1) {
                                frame->pts = frame->best_effort_timestamp;
                            } else if (!decoder_reorder_pts) {
                                frame->pts = frame->pkt_dts;
                            }
                        }
                        break;
                    case AVMEDIA_TYPE_AUDIO:
                        ret = avcodec_receive_frame(d->avctx, frame);
                        if (ret >= 0) {
                            AVRational tb = (AVRational) {1, frame->sample_rate};
                            if (frame->pts != AV_NOPTS_VALUE)
                                frame->pts = av_rescale_q(frame->pts, d->avctx->pkt_timebase, tb);
                            else if (d->next_pts != AV_NOPTS_VALUE)
                                frame->pts = av_rescale_q(d->next_pts, d->next_pts_tb, tb);
                            if (frame->pts != AV_NOPTS_VALUE) {
                                d->next_pts = frame->pts + frame->nb_samples;
                                d->next_pts_tb = tb;
                            }
                        }
                        break;
                }
                if (ret == AVERROR_EOF) {
                    d->finished = d->pkt_serial;
                    avcodec_flush_buffers(d->avctx);
                    return 0;
                }
                if (ret >= 0)
                    return 1;
            } while (ret != AVERROR(EAGAIN));
        }

        do {
            if (d->queue->nb_packets == 0)
                LOGD("NO IMPL");
//                SDL_CondSignal(d->empty_queue_cond);
            if (d->packet_pending) {
                d->packet_pending = 0;
            } else {
                int old_serial = d->pkt_serial;
                if (packet_queue_get(d->queue, d->pkt, 1, &d->pkt_serial) < 0)
                    return -1;
                if (old_serial != d->pkt_serial) {
                    avcodec_flush_buffers(d->avctx);
                    d->finished = 0;
                    d->next_pts = d->start_pts;
                    d->next_pts_tb = d->start_pts_tb;
                }
            }
            if (d->queue->serial == d->pkt_serial)
                break;
            av_packet_unref(d->pkt);
        } while (1);

        if (d->avctx->codec_type == AVMEDIA_TYPE_SUBTITLE) {
            int got_frame = 0;
            ret = avcodec_decode_subtitle2(d->avctx, sub, &got_frame, d->pkt);
            if (ret < 0) {
                ret = AVERROR(EAGAIN);
            } else {
                if (got_frame && !d->pkt->data) {
                    d->packet_pending = 1;
                }
                ret = got_frame ? 0 : (d->pkt->data ? AVERROR(EAGAIN) : AVERROR_EOF);
            }
            av_packet_unref(d->pkt);
        } else {
            if (avcodec_send_packet(d->avctx, d->pkt) == AVERROR(EAGAIN)) {
                av_log(d->avctx, AV_LOG_ERROR,
                       "Receive_frame and send_packet both returned EAGAIN, which is an API violation.\n");
                d->packet_pending = 1;
            } else {
                av_packet_unref(d->pkt);
            }
        }
    }
}

int AudioDecodec::packet_queue_get(PacketQueue *q, AVPacket *pkt, int block, int *serial) {
    MyAVPacketList pkt1;
    int ret;

//    SDL_LockMutex(q->mutex);
    for (;;) {
        if (q->abort_request) {
            ret = -1;
            break;
        }

        if (av_fifo_size(q->pkt_list) >= sizeof(pkt1)) {
            av_fifo_generic_read(q->pkt_list, &pkt1, sizeof(pkt1), NULL);
            q->nb_packets--;
            q->size -= pkt1.pkt->size + sizeof(pkt1);
            q->duration -= pkt1.pkt->duration;
            av_packet_move_ref(pkt, pkt1.pkt);
            if (serial)
                *serial = pkt1.serial;
            av_packet_free(&pkt1.pkt);
            ret = 1;
            break;
        } else if (!block) {
            ret = 0;
            break;
        } else {
//            SDL_CondWait(q->cond, q->mutex);
        }
    }
//    SDL_UnlockMutex(q->mutex);
    return ret;
}

int AudioDecodec::cmp_audio_fmts(enum AVSampleFormat fmt1, int64_t channel_count1,
                                 enum AVSampleFormat fmt2, int64_t channel_count2) {
    /* If channel count == 1, planar and non-planar formats are the same */
    if (channel_count1 == 1 && channel_count2 == 1)
        return av_get_packed_sample_fmt(fmt1) != av_get_packed_sample_fmt(fmt2);
    else
        return channel_count1 != channel_count2 || fmt1 != fmt2;
}

int AudioDecodec::configure_filtergraph(AVFilterGraph *graph, const char *filtergraph,
                                        AVFilterContext *source_ctx, AVFilterContext *sink_ctx) {
    int ret, i;
    int nb_filters = graph->nb_filters;
    AVFilterInOut *outputs = NULL, *inputs = NULL;

    if (filtergraph) {
        outputs = avfilter_inout_alloc();
        inputs = avfilter_inout_alloc();
        if (!outputs || !inputs) {
            ret = AVERROR(ENOMEM);
            goto fail;
        }

        outputs->name = av_strdup("in");
        outputs->filter_ctx = source_ctx;
        outputs->pad_idx = 0;
        outputs->next = NULL;

        inputs->name = av_strdup("out");
        inputs->filter_ctx = sink_ctx;
        inputs->pad_idx = 0;
        inputs->next = NULL;

        if ((ret = avfilter_graph_parse_ptr(graph, filtergraph, &inputs, &outputs, NULL)) < 0)
            goto fail;
    } else {
        if ((ret = avfilter_link(source_ctx, 0, sink_ctx, 0)) < 0)
            goto fail;
    }

    /* Reorder the filters to ensure that inputs of the custom filters are merged first */
    for (i = 0; i < graph->nb_filters - nb_filters; i++)
        FFSWAP(AVFilterContext*, graph->filters[i], graph->filters[i + nb_filters]);

    ret = avfilter_graph_config(graph, NULL);
    fail:
    avfilter_inout_free(&outputs);
    avfilter_inout_free(&inputs);
    return ret;
}

int AudioDecodec::configure_audio_filters(MediaState *is, const char *afilters,
                                          int force_output_format) {
    static const enum AVSampleFormat sample_fmts[] = {AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_NONE};
    int sample_rates[2] = {0, -1};
    int64_t channel_layouts[2] = {0, -1};
    int channels[2] = {0, -1};
    AVFilterContext *filt_asrc = NULL, *filt_asink = NULL;
    char aresample_swr_opts[512] = "";
    const AVDictionaryEntry *e = NULL;
    char asrc_args[256];
    int ret;

    avfilter_graph_free(&is->agraph);
    if (!(is->agraph = avfilter_graph_alloc()))
        return AVERROR(ENOMEM);
    is->agraph->nb_threads = filter_nbthreads;

    while ((e = av_dict_get(swr_opts, "", e, AV_DICT_IGNORE_SUFFIX)))
        av_strlcatf(aresample_swr_opts, sizeof(aresample_swr_opts), "%s=%s:", e->key, e->value);
    if (strlen(aresample_swr_opts))
        aresample_swr_opts[strlen(aresample_swr_opts) - 1] = '\0';
    av_opt_set(is->agraph, "aresample_swr_opts", aresample_swr_opts, 0);

    ret = snprintf(asrc_args, sizeof(asrc_args),
                   "sample_rate=%d:sample_fmt=%s:channels=%d:time_base=%d/%d",
                   is->m_audioObj->audio_filter_src.freq,
                   av_get_sample_fmt_name(is->m_audioObj->audio_filter_src.fmt),
                   is->m_audioObj->audio_filter_src.channels,
                   1, is->m_audioObj->audio_filter_src.freq);
//    if (is->m_audioObj->audio_filter_src.channel_layout)
//        snprintf(asrc_args + ret, sizeof(asrc_args) - ret,
//                 ":channel_layout=0x%"PRIx64,  is->audio_filter_src.channel_layout);

    ret = avfilter_graph_create_filter(&filt_asrc,
                                       avfilter_get_by_name("abuffer"), "ffplay_abuffer",
                                       asrc_args, NULL, is->agraph);
    if (ret < 0)
        goto end;


    ret = avfilter_graph_create_filter(&filt_asink,
                                       avfilter_get_by_name("abuffersink"), "ffplay_abuffersink",
                                       NULL, NULL, is->agraph);
    if (ret < 0)
        goto end;

    if ((ret = av_opt_set_int_list(filt_asink, "sample_fmts", sample_fmts, AV_SAMPLE_FMT_NONE,
                                   AV_OPT_SEARCH_CHILDREN)) < 0)
        goto end;
    if ((ret = av_opt_set_int(filt_asink, "all_channel_counts", 1, AV_OPT_SEARCH_CHILDREN)) < 0)
        goto end;

    if (force_output_format) {
        channel_layouts[0] = is->m_audioObj->audio_tgt.channel_layout;
        channels[0] = is->m_audioObj->audio_tgt.channel_layout ? -1
                                                               : is->m_audioObj->audio_tgt.channels;
        sample_rates[0] = is->m_audioObj->audio_tgt.freq;
        if ((ret = av_opt_set_int(filt_asink, "all_channel_counts", 0, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
        if ((ret = av_opt_set_int_list(filt_asink, "channel_layouts", channel_layouts, -1,
                                       AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
        if ((ret = av_opt_set_int_list(filt_asink, "channel_counts", channels, -1,
                                       AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
        if ((ret = av_opt_set_int_list(filt_asink, "sample_rates", sample_rates, -1,
                                       AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
    }


    if ((ret = configure_filtergraph(is->agraph, afilters, filt_asrc, filt_asink)) < 0)
        goto end;

    is->m_audioObj->in_audio_filter = filt_asrc;
    is->m_audioObj->out_audio_filter = filt_asink;

    end:
    if (ret < 0)
        avfilter_graph_free(&is->agraph);
    return ret;
}

Frame *AudioDecodec::frame_queue_peek_writable(FrameQueue *f) {
//    /* wait until we have space to put a new frame */
//    SDL_LockMutex(f->mutex);
//    while (f->size >= f->max_size &&
//           !f->pktq->abort_request) {
//        SDL_CondWait(f->cond, f->mutex);
//    }
//    SDL_UnlockMutex(f->mutex);
//
//    if (f->pktq->abort_request)
//        return NULL;
//
//    return &f->queue[f->windex];
    return &f->queue[f->windex];
}

void AudioDecodec::onThreadRun(uint32_t now) {
    got_frame = decoder_decode_frame(is->auddec, NULL);

    if (got_frame) {
        tb = (AVRational) {1, frame->sample_rate};
        if (frame->channel_layout &&
            av_get_channel_layout_nb_channels(frame->channel_layout) == frame->channels)
            dec_channel_layout = frame->channel_layout;
        else
            dec_channel_layout = 0;

        AVSampleFormat frameFormat = static_cast<AVSampleFormat>(frame->format);
        reconfigure =
                cmp_audio_fmts(is->m_audioObj->audio_filter_src.fmt,
                               is->m_audioObj->audio_filter_src.channels,
                               frameFormat, frame->channels) ||
                is->m_audioObj->audio_filter_src.channel_layout != dec_channel_layout ||
                is->m_audioObj->audio_filter_src.freq != frame->sample_rate ||
                is->auddec->pkt_serial != last_serial;
        int ret = 0;
        if (reconfigure) {
            char buf1[1024], buf2[1024];
            av_get_channel_layout_string(buf1, sizeof(buf1), -1,
                                         is->m_audioObj->audio_filter_src.channel_layout);
            av_get_channel_layout_string(buf2, sizeof(buf2), -1, dec_channel_layout);
            av_log(NULL, AV_LOG_DEBUG,
                   "Audio frame changed from rate:%d ch:%d fmt:%s layout:%s serial:%d to rate:%d ch:%d fmt:%s layout:%s serial:%d\n",
                   is->m_audioObj->audio_filter_src.freq, is->m_audioObj->audio_filter_src.channels,
                   av_get_sample_fmt_name(is->m_audioObj->audio_filter_src.fmt), buf1, last_serial,
                   frame->sample_rate, frame->channels, av_get_sample_fmt_name(frameFormat), buf2,
                   is->auddec->pkt_serial);

            is->m_audioObj->audio_filter_src.fmt = frameFormat;
            is->m_audioObj->audio_filter_src.channels = frame->channels;
            is->m_audioObj->audio_filter_src.channel_layout = dec_channel_layout;
            is->m_audioObj->audio_filter_src.freq = frame->sample_rate;
            last_serial = is->auddec->pkt_serial;


            if ((ret = configure_audio_filters(is, afilters, 1)) < 0)
//                goto the_end;
                return;
        }

        if ((ret = av_buffersrc_add_frame(is->m_audioObj->in_audio_filter, frame)) < 0)
//            goto the_end;

            while ((ret = av_buffersink_get_frame_flags(is->m_audioObj->out_audio_filter, frame,
                                                        0)) >= 0) {
                tb = av_buffersink_get_time_base(is->m_audioObj->out_audio_filter);
                if (!(af = frame_queue_peek_writable(is->m_audioObj->sampq)))
//                goto the_end;
                    return;
                af->pts = (frame->pts == AV_NOPTS_VALUE) ? NAN : frame->pts * av_q2d(tb);
                af->pos = frame->pkt_pos;
                af->serial = is->auddec->pkt_serial;
                af->duration = av_q2d((AVRational) {frame->nb_samples, frame->sample_rate});

                av_frame_move_ref(af->frame, frame);
//            frame_queue_push(&is->m_audioObj->sampq);

                if (is->m_audioObj->audioq->serial != is->auddec->pkt_serial)
                    break;
            }
        if (ret == AVERROR_EOF)
            is->auddec->finished = is->auddec->pkt_serial;
    }

}