//
// Created by Peter Xi on 2022/6/3.
//

#include "Vout.h"

//
//Vout::Vout() {
//    std::ostringstream logOs;
//    logOs<<"Vout::Vout";
//    video_fq = new FrameQueue();
//    videoq = new PacketQueue();
//    video_fq->pktq = videoq;
//    video_fq->max_size = VIDEO_PICTURE_QUEUE_SIZE;
//    video_fq->keep_last = 1;
//    for (int i = 0; i < video_fq->max_size; i++)
//        if (!(video_fq->queue[i].frame = av_frame_alloc()))
//            logOs<<"fail alloc";
//
//    //    packet_queue_init
//    videoq->pkt_list = av_fifo_alloc(sizeof(MyAVPacketList));
//    if (!videoq->pkt_list) logOs<<"pkt_list fail alloc";
//    videoq->abort_request=1;
//
//    //    init_clock
//    vidclk = new Clock();
//    vidclk->speed = 1.0;
//    vidclk->paused = 0;
//    vidclk->queue_serial = &videoq->serial;
//    //set_clock
//    double time = av_gettime_relative() / 1000000.0;
//    vidclk->pts = NAN;
//    vidclk->last_updated = time;
//    vidclk->pts_drift = vidclk->pts - time;
//    vidclk->serial = -1;
//
//
//
//
//}
//
//Vout::~Vout() {
//    SafeDelete(video_fq);
//    SafeDelete(videoq);
//}