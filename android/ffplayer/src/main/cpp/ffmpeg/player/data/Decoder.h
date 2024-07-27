//
// Created by Administrator on 2024/7/27.
//

#ifndef JUSTPLAYER_DECODER_H
#define JUSTPLAYER_DECODER_H

#include "FFinc.h"
#include "PacketQueue.h"

class Decoder {
public:
    AVPacket *pkt;
    PacketQueue *queue;
    AVCodecContext *avctx;
    int pkt_serial;
    int finished;
    int packet_pending;
//    SDL_cond *empty_queue_cond;
    int64_t start_pts;
    AVRational start_pts_tb;
    int64_t next_pts;
    AVRational next_pts_tb;
//    SDL_Thread *decoder_tid;

};


#endif //JUSTPLAYER_DECODER_H
