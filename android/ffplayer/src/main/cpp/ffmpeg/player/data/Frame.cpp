//
// Created by Administrator on 2024/7/28.
//

#include "Frame.h"

Frame::Frame() {

}

Frame::~Frame() {
    av_frame_free(&frame);
//    if (vp->bmp) {
//        SDL_VoutFreeYUVOverlay(vp->bmp);
//        vp->bmp = NULL;
//    }
}


