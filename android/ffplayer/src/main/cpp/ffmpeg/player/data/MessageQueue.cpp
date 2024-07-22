//
// Created by Administrator on 2024/7/22.
//

#include "MessageQueue.h"
#include "../../ffmpeginc/libavutil/mem.h"


MessageQueue::MessageQueue() {

}

MessageQueue::~MessageQueue() {

}

void MessageQueue::freeMessage(AVMessage *msg) {
    if (!msg || !msg->obj)
        return;
//    assert(msg->free_l); todo
    msg->free_l(msg->obj);
    msg->obj = nullptr;
}

int MessageQueue::putMessagePrivate(AVMessage *msg) {
    AVMessage *msg1;

    if (abort_request)
        return -1;

#ifdef FFP_MERGE
    msg1 = av_malloc(sizeof(AVMessage));
#else
    msg1 = recycle_msg;
    if (msg1) {
        recycle_msg = msg1->next;
        recycle_count++;
    } else {
        alloc_count++;
        msg1 = new AVMessage();
    }
#ifdef FFP_SHOW_MSG_RECYCLE
    int total_count = recycle_count + alloc_count;
    if (!(total_count % 10)) {
        av_log(NULL, AV_LOG_DEBUG, "msg-recycle \t%d + \t%d = \t%d\n", recycle_count, alloc_count, total_count);
    }
#endif
#endif
    if (!msg1)
        return -1;

    *msg1 = *msg;
    msg1->next = NULL;

    if (!last_msg)
        first_msg = msg1;
    else
        last_msg->next = msg1;
    last_msg = msg1;
    nb_messages++;
//    SDL_CondSignal(cond);
    return 0;
}

int MessageQueue::putMessage(AVMessage *msg) {
    int ret;

//    SDL_LockMutex(mutex);
    ret = putMessagePrivate(msg);
//    SDL_UnlockMutex(mutex);

    return ret;
}

void MessageQueue::initMessage(AVMessage *msg) {
    memset(msg, 0, sizeof(AVMessage));
}

void MessageQueue::putEmptyMessage(int what, int arg1, int arg2, void *obj, int obj_len) {
    AVMessage msg;
    initMessage(&msg);
    msg.what = what;
    msg.arg1 = arg1;
    msg.arg2 = arg2;
    msg.obj = av_malloc(obj_len);
    memcpy(msg.obj, obj, obj_len);
    msg.free_l = av_free;
    putMessage(&msg);
}

void MessageQueue::initMsgQueue() {
    memset(this, 0, sizeof(MessageQueue));
//    q->mutex = SDL_CreateMutex();
//    q->cond = SDL_CreateCond();
    abort_request = 1;
}

void MessageQueue::flushMsgQueue() {
    AVMessage *msg, *msg1;

//    SDL_LockMutex(mutex);
    for (msg = first_msg; msg != NULL; msg = msg1) {
        msg1 = msg->next;
#ifdef FFP_MERGE
        av_freep(&msg);
#else
        msg->next = recycle_msg;
        recycle_msg = msg;
#endif
    }
    last_msg = NULL;
    first_msg = NULL;
    nb_messages = 0;
//    SDL_UnlockMutex(mutex);
}

void MessageQueue::destroyMsgQueue() {
    flushMsgQueue();

//    SDL_LockMutex(mutex);
    while(recycle_msg) {
        AVMessage *msg = recycle_msg;
        if (msg)
            recycle_msg = msg->next;
        freeMessage(msg);
        av_freep(&msg);
    }
//    SDL_UnlockMutex(q->mutex);
//    SDL_DestroyMutex(q->mutex);
//    SDL_DestroyCond(q->cond);
}

void MessageQueue::abortMsgQueue() {
//    SDL_LockMutex(q->mutex);
    abort_request = 1;
//    SDL_CondSignal(q->cond);
//    SDL_UnlockMutex(q->mutex);
}

void MessageQueue::startMsgQueue() {
//    SDL_LockMutex(mutex);
    abort_request = 0;

    AVMessage msg;
    initMessage(&msg);
    msg.what = FFP_MSG_FLUSH;
    putMessagePrivate(&msg);
//    SDL_UnlockMutex(q->mutex);
}

int MessageQueue::getMsgQueue(AVMessage *msg, int block) {
    AVMessage *msg1;
    int ret;

//    SDL_LockMutex(q->mutex);

    for (;;) {
        if (abort_request) {
            ret = -1;
            break;
        }

        msg1 = first_msg;
        if (msg1) {
            first_msg = msg1->next;
            if (!first_msg)
                last_msg = NULL;
            nb_messages--;
            *msg = *msg1;
            msg1->obj = NULL;
#ifdef FFP_MERGE
            av_free(msg1);
#else
            msg1->next = recycle_msg;
            recycle_msg = msg1;
#endif
            ret = 1;
            break;
        } else if (!block) {
            ret = 0;
            break;
        } else {
//            SDL_CondWait(cond, mutex);
        }
    }
//    SDL_UnlockMutex(q->mutex);
    return ret;
}

void MessageQueue::removeMsgQueue(int what) {
    AVMessage **p_msg, *msg, *last_msg;
//    SDL_LockMutex(q->mutex);

    last_msg = first_msg;

    if (!abort_request && first_msg) {
        p_msg = &first_msg;
        while (*p_msg) {
            msg = *p_msg;

            if (msg->what == what) {
                *p_msg = msg->next;
#ifdef FFP_MERGE
                av_free(msg);
#else
                freeMessage(msg);
                msg->next = recycle_msg;
                recycle_msg = msg;
#endif
                nb_messages--;
            } else {
                last_msg = msg;
                p_msg = &msg->next;
            }
        }

        if (first_msg) {
            last_msg = last_msg;
        } else {
            last_msg = NULL;
        }
    }

//    SDL_UnlockMutex(mutex);
}
























