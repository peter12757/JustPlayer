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
        SafeDelete(vp);
    }
//    SDL_DestroyMutex(mutex);
//    SDL_DestroyCond(cond);
}

void FrameQueue::frame_queue_unref_item(Frame *vp) {
    av_frame_unref(vp->frame);
//    SDL_VoutUnrefYUVOverlay(vp->bmp);
    avsubtitle_free(&vp->sub);
}

void FrameQueue::frame_queue_signal() {
//    SDL_LockMutex(mutex);
//    SDL_CondSignal(cond);
//    SDL_UnlockMutex(mutex);
}

Frame *FrameQueue::frame_queue_peek() {
    return &queue[(rindex + rindex_shown) % max_size];
}

Frame *FrameQueue::frame_queue_peek_next() {
    return &queue[(rindex + rindex_shown + 1) % max_size];
}

Frame *FrameQueue::frame_queue_peek_last() {
    return &queue[rindex];
}

Frame *FrameQueue::frame_queue_peek_writable() {
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

Frame *FrameQueue::frame_queue_peek_readable() {
    /* wait until we have a readable a new frame */
//    SDL_LockMutex(mutex);
    while (size - rindex_shown <= 0 &&
           !pktq->abort_request) {
//        SDL_CondWait(cond, mutex);
    }
//    SDL_UnlockMutex(mutex);

    if (pktq->abort_request)
        return NULL;

    return &queue[(rindex + rindex_shown) % max_size];
}

void FrameQueue::frame_queue_push() {
    if (++windex == max_size)
        windex = 0;
//    SDL_LockMutex(mutex);
    size++;
//    SDL_CondSignal(cond);
//    SDL_UnlockMutex(mutex);
}

void FrameQueue::frame_queue_next() {
    // 如果支持keep_last，且rindex_shown为0，则rindex_shown赋1，返回。
    // 反之不支持keep_last的话，rindex_shown永远都是0，因为只有这一个地方是对rindex_shown赋值的。
    if (keep_last && !rindex_shown) {
        rindex_shown = 1;
        return;
    }

    // 已经读过的节点，释放关联内存
    frame_queue_unref_item(&queue[rindex]);
    //如果超过了max_size，则回环为0
    if (++rindex == max_size)
        rindex = 0;

    // 当前存储的节点个数size 减 1
//    SDL_LockMutex(mutex);
    size--;
//    SDL_CondSignal(cond);
//    SDL_UnlockMutex(mutex);

}

#ifdef FFP_MERGE
/* return last shown position */
int64_t FrameQueue::frame_queue_last_pos() {
    Frame *fp = &queue[rindex];
    if (rindex_shown && fp->serial == pktq->serial)
        return fp->pos;
    else
        return -1;
}
#endif












