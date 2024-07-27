//
// Created by Administrator on 2024/7/27.
//

#include "PacketQueue.h"

PacketQueue::PacketQueue() {
    memset(this, 0, sizeof(PacketQueue));
//    mutex = SDL_CreateMutex();
//    if (!q->mutex) {
//        av_log(NULL, AV_LOG_FATAL, "SDL_CreateMutex(): %s\n", SDL_GetError());
//        return AVERROR(ENOMEM);
//    }
//    q->cond = SDL_CreateCond();
//    if (!q->cond) {
//        av_log(NULL, AV_LOG_FATAL, "SDL_CreateCond(): %s\n", SDL_GetError());
//        return AVERROR(ENOMEM);
//    }
    abort_request = 1;
}


