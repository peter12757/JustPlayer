//
// Created by Administrator on 2024/7/27.
//

#ifndef JUSTPLAYER_FRAMEQUEUE_H
#define JUSTPLAYER_FRAMEQUEUE_H

#include "FFinc.h"
#include "PacketQueue.h"
#include "Frame.h"
#include "LogUtil.h"
#include "common.h"


// #define VIDEO_PICTURE_QUEUE_SIZE 3
#define VIDEO_PICTURE_QUEUE_SIZE_MIN        (3)
#define VIDEO_PICTURE_QUEUE_SIZE_MAX        (16)
#define VIDEO_PICTURE_QUEUE_SIZE_DEFAULT    (VIDEO_PICTURE_QUEUE_SIZE_MIN)
#define SUBPICTURE_QUEUE_SIZE 16
#define SAMPLE_QUEUE_SIZE 9
#define FRAME_QUEUE_SIZE FFMAX(SAMPLE_QUEUE_SIZE, FFMAX(VIDEO_PICTURE_QUEUE_SIZE_MAX, SUBPICTURE_QUEUE_SIZE))

class FrameQueue {
public:
    Frame queue[FRAME_QUEUE_SIZE];
    int rindex;
    int windex;
    int size;
    int max_size;
    int keep_last;
    int rindex_shown;
    PacketQueue* pktq;

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

    void frame_queue_unref_item(Frame *vp);
    void frame_queue_signal();
    Frame *frame_queue_peek();
    Frame *frame_queue_peek_next();
    Frame *frame_queue_peek_last();
    Frame *frame_queue_peek_writable();
    Frame *frame_queue_peek_readable();
    void frame_queue_push();
    void frame_queue_next();
#ifdef FFP_MERGE
    int64_t frame_queue_last_pos();
#endif
};


#endif //JUSTPLAYER_FRAMEQUEUE_H
