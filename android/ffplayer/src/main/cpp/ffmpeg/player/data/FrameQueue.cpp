//
// Created by Administrator on 2024/7/27.
//

#include "FrameQueue.h"

Frame *FrameQueue::peekWritable(FrameQueue *f) {
    /* wait until we have space to put a new frame */
//    SDL_LockMutex(f->mutex);
    while (f->size >= f->max_size &&
           !f->pktq->abort_request) {
//        SDL_CondWait(f->cond, f->mutex);
    }
//    SDL_UnlockMutex(f->mutex);

    if (f->pktq->abort_request)
        return nullptr;

    return &f->queue[f->windex];
}

void FrameQueue::push(FrameQueue *f) {

}
