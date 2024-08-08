//
// Created by Administrator on 2024/8/5.
//

#ifndef JUSTPLAYER_VIDEOSTATE_H
#define JUSTPLAYER_VIDEOSTATE_H

#include <string>
#include "FFinc.h"
#include "FrameQueue.h"
#include "PacketQueue.h"
#include "Clock.h"

class VideoState {

public:
    std::string datasourcel;

    AVInputFormat *iformat;

    int width, height, xleft, ytop;


    FrameQueue *pictq;
    PacketQueue *videoq;
    Clock *vidclk;
    int pictq_size;

    FrameQueue *subpq;
    PacketQueue *subtitleq;

    FrameQueue *sampq;
    PacketQueue *audioq;
    Clock *audclk;
    int audio_clock_serial;
    int audio_volume;


    Clock *extclk;

    bool abort_request;
    bool pause_req;

    VideoState(std::string url,AVInputFormat *iformat,,int av_sync_type);
    ~VideoState();


    void reset();

};


#endif //JUSTPLAYER_VIDEOSTATE_H
