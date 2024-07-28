//
// Created by Administrator on 2024/7/27.
//

#ifndef JUSTPLAYER_FRAMEQUEUE_H
#define JUSTPLAYER_FRAMEQUEUE_H

#include "FFinc.h"
#include "PacketQueue.h"
#include "Frame.h"

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

    /* return last shown position */
#ifdef FFP_MERGE
    int64_t frame_queue_last_pos()
{
    Frame *fp = &queue[rindex];
    if (rindex_shown && fp->serial == pktq->serial)
        return fp->pos;
    else
        return -1;
}
#endif

    Frame    *peekWritable();
    void      push();

    void frame_queue_unref_item(Frame *vp);
    void frame_queue_signal(FrameQueue *f);
    Frame *frame_queue_peek(FrameQueue *f);
    Frame *frame_queue_peek_next(FrameQueue *f);
    Frame *frame_queue_peek_last(FrameQueue *f);
    Frame *frame_queue_peek_writable(FrameQueue *f);
    Frame *frame_queue_peek_readable(FrameQueue *f);
    void frame_queue_push(FrameQueue *f);
    void frame_queue_next(FrameQueue *f);
    int64_t frame_queue_last_pos(FrameQueue *f);
};


#endif //JUSTPLAYER_FRAMEQUEUE_H
