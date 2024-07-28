//
// Created by Administrator on 2024/7/27.
//

#include "FrameQueue.h"

FrameQueue::FrameQueue() {
    int i;
    memset(this, 0, sizeof(FrameQueue));
//    if (!(mutex = SDL_CreateMutex())) {
//        av_log(NULL, AV_LOG_FATAL, "SDL_CreateMutex(): %s\n", SDL_GetError());
//        return AVERROR(ENOMEM);
//    }
//    if (!(cond = SDL_CreateCond())) {
//        av_log(NULL, AV_LOG_FATAL, "SDL_CreateCond(): %s\n", SDL_GetError());
//        return AVERROR(ENOMEM);
//    }
    pktq = pktq;
    max_size = FFMIN(max_size, FRAME_QUEUE_SIZE);
    keep_last = !!keep_last;
    for (i = 0; i < max_size; i++)
        if (!(queue[i].frame = av_frame_alloc())) {
//            LOGE(ENOMEM);
    AVERROR(ENOMEM);
}
}

FrameQueue::~FrameQueue() {
    int i;
    for (i = 0; i < max_size; i++) {
        Frame *vp = &queue[i];
        frame_queue_unref_item(vp);
        SafeDelete(vp)
    }
//    SDL_DestroyMutex(mutex);
//    SDL_DestroyCond(cond);
}


Frame *FrameQueue::peekWritable() {
    /* wait until we have space to put a new frame */
//    SDL_LockMutex(mutex);
    while (size >= max_size &&
           !pktq->abort_request) {
//        SDL_CondWait(cond, mutex);
    }
//    SDL_UnlockMutex(mutex);

    if (pktq->abort_request)
        return nullptr;

    return &queue[windex];
}

void FrameQueue::push() {
    if (++windex == max_size)
        windex = 0;
//    SDL_LockMutex(mutex);
    size++;
//    SDL_CondSignal(cond);
//    SDL_UnlockMutex(mutex);
}

void FrameQueue::frame_queue_unref_item(Frame *vp) {
    av_frame_unref(vp->frame);
//    SDL_VoutUnrefYUVOverlay(vp->bmp);
    avsubtitle_free(&vp->sub);
}







