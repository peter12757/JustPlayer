//
// Created by Administrator on 2024/7/27.
//

#ifndef JUSTPLAYER_DECODER_H
#define JUSTPLAYER_DECODER_H

#include "FFinc.h"
#include "PacketQueue.h"
#include "FrameQueue.h"
#include "Profiler.h"

#define FFP_XPS_PERIOD (3)

#define FFP_SHOW_VPS_DPS            (0)
#define FFP_SHOW_VFPS_FFPLAY        (FFP_SHOW_VPS_DPS)
#define FFP_SHOW_VDPS_AVCODEC       (FFP_SHOW_VPS_DPS)
#define FFP_SHOW_VDPS_VIDEOTOOLBOX  (FFP_SHOW_VPS_DPS)
#define FFP_SHOW_VDPS_MEDIACODEC    (FFP_SHOW_VPS_DPS)


class Decoder {
public:
    AVPacket pkt;
    AVPacket pkt_temp;
    PacketQueue *queue;
    AVCodecContext *avctx;
    int pkt_serial;
    int finished;
    int packet_pending;
    int bfsc_ret;
    uint8_t *bfsc_data;

//    SDL_cond *empty_queue_cond;
    int64_t start_pts;
    AVRational start_pts_tb;
    int64_t next_pts;
    AVRational next_pts_tb;
//    SDL_Thread *decoder_tid;
//    SDL_Thread _decoder_tid;

    Profiler *decode_profiler;
    uint64_t first_frame_decoded_time;
    int    first_frame_decoded;


public:
    Decoder(AVCodecContext *avctx,PacketQueue *queue);
    ~Decoder();


    void decoder_abort(FrameQueue *fq);

    int decoder_start(int (*fn)(void *), void *arg, const char *name);

};


#endif //JUSTPLAYER_DECODER_H
