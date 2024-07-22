//
// Created by Peter Xi on 2022/6/3.
//

#include "Aout.h"


Aout::Aout()
{
    std::ostringstream logOs;
    logOs<<"Aout::Aout";
    sampq = new FrameQueue();
    audioq = new PacketQueue();
    sampq->pktq = audioq;
    sampq->max_size = SAMPLE_QUEUE_SIZE;
    sampq->keep_last = 1;
    for (int i = 0; i < sampq->max_size; i++)
        if (!(sampq->queue[i].frame = av_frame_alloc()))
            logOs<<"fail alloc";

    //    packet_queue_init
    audioq->pkt_list = av_fifo_alloc(sizeof(MyAVPacketList));
    if (!audioq->pkt_list) logOs<<"pkt_list fail alloc";
    audioq->abort_request=1;

    //    init_clock
    audclk = new Clock();
    audclk->speed = 1.0;
    audclk->paused = 0;
    audclk->queue_serial = &audioq->serial;
    //set_clock
    double time = av_gettime_relative() / 1000000.0;
    audclk->pts = NAN;
    audclk->last_updated = time;
    audclk->pts_drift = audclk->pts - time;
    audclk->serial = -1;

    audio_clock_serial = -1;

    audio_volume = 100; //def
    muted = false;

}

Aout::~Aout() {
    SafeDelete(sampq);
    SafeDelete(audioq);
}