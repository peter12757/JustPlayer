//
// Created by Administrator on 2024/7/28.
//

#ifndef JUSTPLAYER_FRAME_H
#define JUSTPLAYER_FRAME_H

#include "FFinc.h"

/* Common struct for handling all types of decoded data and allocated render buffers. */
class Frame {
public:
    AVFrame *frame;
    AVSubtitle sub;
    int serial;
    double pts;           /* presentation timestamp for the frame */
    double duration;      /* estimated duration of the frame */
    int64_t pos;          /* byte position of the frame in the input file */
#ifdef FFP_MERGE
    SDL_Texture *bmp;
#else
//    SDL_VoutOverlay *bmp;
#endif
    int allocated;
    int width;
    int height;
    int format;
    AVRational sar;
    int uploaded;

public:
    Frame();
    ~Frame();
};


#endif //JUSTPLAYER_FRAME_H
