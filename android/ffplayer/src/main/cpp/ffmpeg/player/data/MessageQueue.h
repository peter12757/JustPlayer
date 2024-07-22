//
// Created by Administrator on 2024/7/22.
//

#ifndef JUSTPLAYER_MESSAGEQUEUE_H
#define JUSTPLAYER_MESSAGEQUEUE_H



#include "FFinc.h"
#include "FFMsg.h"

typedef struct AVMessage {
    int what;
    int arg1;
    int arg2;
    void *obj;
    void (*free_l)(void *obj);
    struct AVMessage *next;
} AVMessage;

class MessageQueue {
public:
    AVMessage *first_msg, *last_msg;
    int nb_messages;
    int abort_request;
//    SDL_mutex *mutex;
//    SDL_cond *cond;

    AVMessage *recycle_msg;
    int recycle_count;
    int alloc_count;

    MessageQueue();
    ~MessageQueue();
    void freeMessage(AVMessage *msg);
    int putMessagePrivate(AVMessage *msg);

    int putMessage(AVMessage *msg);
    void initMessage(AVMessage *msg);
    void putEmptyMessage(int what, int arg1 =INT32_MIN, int arg2 = INT32_MIN, void *obj= nullptr, int obj_len = 0);

    void initMsgQueue();
    void flushMsgQueue();
    void destroyMsgQueue();
    void abortMsgQueue();
    void startMsgQueue();
    int getMsgQueue(AVMessage *msg, int block);
    void removeMsgQueue(int what);
} MessageQueue;


#endif //JUSTPLAYER_MESSAGEQUEUE_H
