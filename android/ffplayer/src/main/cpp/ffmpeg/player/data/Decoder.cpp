//
// Created by Administrator on 2024/7/27.
//

#include "Decoder.h"


Decoder::Decoder(AVCodecContext *avctx, PacketQueue *queue):
avctx(avctx),
queue(queue)
{
//    memset(this, 0, sizeof(Decoder));
//    empty_queue_cond = empty_queue_cond;
    start_pts = AV_NOPTS_VALUE;

    first_frame_decoded_time = decode_profiler->getTickHR();
    first_frame_decoded = 0;

    decode_profiler = new Profiler(-1);
}

Decoder::~Decoder() {
    av_packet_unref(&pkt);
    avcodec_free_context(&avctx);
}

int Decoder::decoder_decode_frame(AVFrame *frame, AVSubtitle *sub) {
    int got_frame = 0;

    do {
        int ret = -1;

        if (queue->abort_request)
            return -1;

        if (!packet_pending || queue->serial != pkt_serial) {
            AVPacket pkt;
            do {
//                if (queue->nb_packets == 0)
//                    SDL_CondSignal(empty_queue_cond);
                if (queue->packet_queue_get_or_buffering(&pkt, &pkt_serial, &finished) < 0)
                    return -1;
                if (pkt.data == flush_pkt->data) {
                    avcodec_flush_buffers(avctx);
                    finished = 0;
                    next_pts = start_pts;
                    next_pts_tb = start_pts_tb;
                }
            } while (pkt.data == flush_pkt.data || queue->serial != pkt_serial);
            av_packet_unref(&pkt);
            pkt_temp = pkt = pkt;
            packet_pending = 1;
        }

        switch (avctx->codec_type) {
            case AVMEDIA_TYPE_VIDEO: {
                ret = avcodec_decode_video2(avctx, frame, &got_frame, &pkt_temp);
                if (got_frame) {
                    stat.vdps = SDL_SpeedSamplerAdd(&vdps_sampler, FFP_SHOW_VDPS_AVCODEC, "vdps[avcodec]");
                    if (decoder_reorder_pts == -1) {
                        frame->pts = av_frame_get_best_effort_timestamp(frame);
                    } else if (!decoder_reorder_pts) {
                        frame->pts = frame->pkt_dts;
                    }
                }
            }
                break;
            case AVMEDIA_TYPE_AUDIO:
                ret = avcodec_decode_audio4(avctx, frame, &got_frame, &pkt_temp);
                if (got_frame) {
                    AVRational tb = (AVRational){1, frame->sample_rate};
                    if (frame->pts != AV_NOPTS_VALUE)
                        frame->pts = av_rescale_q(frame->pts, av_codec_get_pkt_timebase(avctx), tb);
                    else if (next_pts != AV_NOPTS_VALUE)
                        frame->pts = av_rescale_q(next_pts, next_pts_tb, tb);
                    if (frame->pts != AV_NOPTS_VALUE) {
                        next_pts = frame->pts + frame->nb_samples;
                        next_pts_tb = tb;
                    }
                }
                break;
            case AVMEDIA_TYPE_SUBTITLE:
                ret = avcodec_decode_subtitle2(avctx, sub, &got_frame, &pkt_temp);
                break;
            default:
                break;
        }

        if (ret < 0) {
            packet_pending = 0;
        } else {
            pkt_temp.dts =
            pkt_temp.pts = AV_NOPTS_VALUE;
            if (pkt_temp.data) {
                if (avctx->codec_type != AVMEDIA_TYPE_AUDIO)
                    ret = pkt_temp.size;
                pkt_temp.data += ret;
                pkt_temp.size -= ret;
                if (pkt_temp.size <= 0)
                    packet_pending = 0;
            } else {
                if (!got_frame) {
                    packet_pending = 0;
                    finished = pkt_serial;
                }
            }
        }
    } while (!got_frame && !finished);

    return got_frame;
}

void Decoder::decoder_abort(FrameQueue *fq) {
    queue->abort();
    fq->frame_queue_signal();
//    SDL_WaitThread(decoder_tid, NULL);
//    decoder_tid = NULL;
    queue->flush();
}

int Decoder::decoder_start(int (*fn)(void *), void *arg, const char *name) {
    queue->start();
//    decoder_tid = SDL_CreateThreadEx(&_decoder_tid, fn, arg, name);
//    if (!decoder_tid) {
//        av_log(NULL, AV_LOG_ERROR, "SDL_CreateThread(): %s\n", SDL_GetError());
//        return AVERROR(ENOMEM);
//    }
    return 0;
}


