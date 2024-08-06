//
// Created by Administrator on 2024/8/5.
//

#ifndef JUSTPLAYER_VIDEOSTATE_H
#define JUSTPLAYER_VIDEOSTATE_H

#include <string>
#include "FFinc.h"
#include "FrameQueue.h"
#include "PacketQueue.h"

class VideoState {

public:
    std::string datasourcel;

    AVInputFormat *iformat;

    int width, height, xleft, ytop;


    FrameQueue pictq;
    PacketQueue videoq;
    int pictq_size;

    FrameQueue subpq;
    FrameQueue sampq;


    VideoState(std::string url,AVInputFormat *iformat);
    ~VideoState();

};


#endif //JUSTPLAYER_VIDEOSTATE_H
