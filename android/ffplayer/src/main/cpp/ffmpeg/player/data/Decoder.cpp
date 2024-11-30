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

//int Decoder::decoder_decode_frame(FFPlayer *ffp,AVFrame *frame, AVSubtitle *sub) {
//    int ret = AVERROR(EAGAIN);
//
//    for (;;) {
//        AVPacket pkt;
//
//        if (queue->serial == pkt_serial) {
//            do {
//                if (queue->abort_request)
//                    return -1;
//
//                switch (avctx->codec_type) {
//                    case AVMEDIA_TYPE_VIDEO:
//                        ret = avcodec_receive_frame(avctx, frame);
//                        if (ret >= 0) {
//                            ffp->stat->vdps = ffp->vdps_sampler.addSpeedSampler(FFP_SHOW_VDPS_AVCODEC, "vdps[avcodec]");
//                            if (ffp->decoder_reorder_pts == -1) {
//                                frame->pts = frame->best_effort_timestamp;
//                            } else if (!ffp->decoder_reorder_pts) {
//                                frame->pts = frame->pkt_dts;
//                            }
//                        }
//                        break;
//                    case AVMEDIA_TYPE_AUDIO:
//                        ret = avcodec_receive_frame(avctx, frame);
//                        if (ret >= 0) {
//                            AVRational tb = (AVRational){1, frame->sample_rate};
//                            if (frame->pts != AV_NOPTS_VALUE)
//                                frame->pts = av_rescale_q(frame->pts, av_codec_get_pkt_timebase(avctx), tb);
//                            else if (next_pts != AV_NOPTS_VALUE)
//                                frame->pts = av_rescale_q(next_pts, next_pts_tb, tb);
//                            if (frame->pts != AV_NOPTS_VALUE) {
//                                next_pts = frame->pts + frame->nb_samples;
//                                next_pts_tb = tb;
//                            }
//                        }
//                        break;
//                    default:
//                        break;
//                }
//                if (ret == AVERROR_EOF) {
//                    finished = pkt_serial;
//                    avcodec_flush_buffers(avctx);
//                    return 0;
//                }
//                if (ret >= 0)
//                    return 1;
//            } while (ret != AVERROR(EAGAIN));
//        }
//
//        do {
//            if (queue->nb_packets == 0){
////                SDL_CondSignal(empty_queue_cond);
//            }
//            if (packet_pending) {
//                av_packet_move_ref(&pkt, &pkt);
//                packet_pending = 0;
//            } else {
//                if (ffp->packet_queue_get_or_buffering(queue, &pkt, &pkt_serial, &finished) < 0)
//                    return -1;
//            }
//        } while (queue->serial != pkt_serial);
//
//        if (pkt.data == flush_pkt->data) {
//            avcodec_flush_buffers(avctx);
//            finished = 0;
//            next_pts = start_pts;
//            next_pts_tb = start_pts_tb;
//        } else {
//            if (avctx->codec_type == AVMEDIA_TYPE_SUBTITLE) {
//                int got_frame = 0;
//                ret = avcodec_decode_subtitle2(avctx, sub, &got_frame, &pkt);
//                if (ret < 0) {
//                    ret = AVERROR(EAGAIN);
//                } else {
//                    if (got_frame && !pkt.data) {
//                        packet_pending = 1;
//                        av_packet_move_ref(&pkt, &pkt);
//                    }
//                    ret = got_frame ? 0 : (pkt.data ? AVERROR(EAGAIN) : AVERROR_EOF);
//                }
//            } else {
//                if (avcodec_send_packet(avctx, &pkt) == AVERROR(EAGAIN)) {
//                    av_log(avctx, AV_LOG_ERROR, "Receive_frame and send_packet both returned EAGAIN, which is an API violation.\n");
//                    packet_pending = 1;
//                    av_packet_move_ref(&pkt, &pkt);
//                }
//            }
//            av_packet_unref(&pkt);
//        }
//    }
//}

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


