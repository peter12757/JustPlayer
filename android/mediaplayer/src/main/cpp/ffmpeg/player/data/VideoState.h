//
// Created by Administrator on 2026/10/27.
//

#ifndef JUSTPLAYER_VIDEOSTATE_H
#define JUSTPLAYER_VIDEOSTATE_H


#include "libavfilter/avfilter.h"
#include "libavutil/tx.h"
#include "config/FFdef.h"
#include "libavformat/avformat.h"

class VideoState {

public:
    VideoState();
    virtual ~VideoState();

    int width{}, height{}, xleft{}, ytop{};
    int step{};
};


#endif //JUSTPLAYER_VIDEOSTATE_H
