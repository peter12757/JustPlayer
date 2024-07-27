//
// Created by Administrator on 2024/7/27.
//

#ifndef JUSTPLAYER_FRAMEQUEUE_H
#define JUSTPLAYER_FRAMEQUEUE_H

#include "FFinc.h"
#include "PacketQueue.h"

class FrameQueue {
public:
    Frame queue[FRAME_QUEUE_SIZE];
    int rindex;
    int windex;
    int size;
    int max_size;
    int keep_last;
    int rindex_shown;
    PacketQueue *pktq;

    FrameQueue();
    ~FrameQueue();

    /* return the number of undisplayed frames in the queue */
    int frame_queue_nb_remaining()
    {
        return size - rindex_shown;
    }

    Frame    *peekWritable();
    void      push();

    void frame_queue_unref_item(Frame *vp);
    int frame_queue_init(FrameQueue *f, PacketQueue *pktq, int max_size, int keep_last);
    void frame_queue_destory(FrameQueue *f);
    void frame_queue_signal(FrameQueue *f);
    Frame *frame_queue_peek(FrameQueue *f);
    Frame *frame_queue_peek_next(FrameQueue *f);
    Frame *frame_queue_peek_last(FrameQueue *f);
    Frame *frame_queue_peek_writable(FrameQueue *f);
    Frame *frame_queue_peek_readable(FrameQueue *f);
    void frame_queue_push(FrameQueue *f);
    void frame_queue_next(FrameQueue *f);
    int frame_queue_nb_remaining(FrameQueue *f);
    int64_t frame_queue_last_pos(FrameQueue *f);
};


#endif //JUSTPLAYER_FRAMEQUEUE_H
