//
// Created by Peter Xi on 2022/6/3.
//

#include "SubtitleObj.h"


SubtitleObj::SubtitleObj() {
    std::ostringstream logOs;
    logOs<<"AudioObj::AudioObj";
    subpq = new FrameQueue();
    subtitleq = new PacketQueue();
    subpq->pktq = subtitleq;
    subpq->max_size = SUBPICTURE_QUEUE_SIZE;
    subpq->keep_last = 0;
    for (int i = 0; i < subpq->max_size; i++)
        if (!(subpq->queue[i].frame = av_frame_alloc()))
            logOs<<"frame fail alloc";

//    packet_queue_init
    subtitleq->pkt_list = av_fifo_alloc(sizeof(MyAVPacketList));
    if (!subtitleq->pkt_list) logOs<<"pkt_list fail alloc";
    subtitleq->abort_request=1;

    //    init_clock
    extclk = new Clock();
    extclk->speed = 1.0;
    extclk->paused = 0;
    extclk->queue_serial = &subtitleq->serial;
    //set_clock
    double time = av_gettime_relative() / 1000000.0;
    extclk->pts = NAN;
    extclk->last_updated = time;
    extclk->pts_drift = extclk->pts - time;
    extclk->serial = -1;


}


SubtitleObj::~SubtitleObj() {
    SafeDelete(subpq);
    SafeDelete(subtitleq);
}