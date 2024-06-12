//
// Created by Peter Xi on 2022/6/3.
//

#ifndef ANDROIDTEST_FFINC_H
#define ANDROIDTEST_FFINC_H
extern "C"{
#include "../ffmpeginc/libavutil/avutil.h"
#include "../ffmpeginc/libavcodec/avcodec.h"
#include "../ffmpeginc/libavformat/avformat.h"
#include "../ffmpeginc/libavdevice/avdevice.h"
#include "../ffmpeginc/libavfilter/avfilter.h"
#include "../ffmpeginc/libavutil/log.h"
#include "../ffmpeginc/libavcodec/avfft.h"
#include "../ffmpeginc/libavutil/fftime.h"
#include "../ffmpeginc/libavutil/dict.h"
#include "../ffmpeginc/libavutil/channel_layout.h"
#include "../ffmpeginc/libavutil/samplefmt.h"
#include "../ffmpeginc/libavfilter/buffersrc.h"
#include "../ffmpeginc/libavfilter/buffersink.h"
#include "../ffmpeginc/libavutil/fifo.h"

#include "../ffmpeginc/libavutil/file.h"
#include "../ffmpeginc/libavutil/mem.h"
#include "../ffmpeginc/libavutil/avstring.h"
}
#include <sstream>
#include "../ffmpeginc/libavutil/fifo.h"
#include "FFConfig.h"


#define MAX_QUEUE_SIZE (15 * 1024 * 1024)
#define MIN_FRAMES 25

/* Common struct for handling all types of decoded data and allocated render buffers. */
struct Frame {
    AVFrame *frame;
    AVSubtitle sub;
    int serial;
    double pts;           /* presentation timestamp for the frame */
    double duration;      /* estimated duration of the frame */
    int64_t pos;          /* byte position of the frame in the input file */
    int width;
    int height;
    int format;
    AVRational sar;
    int uploaded;
    int flip_v;
};

struct MyAVPacketList {
    AVPacket *pkt;
    int serial;
};

enum AVSyncType{
    AV_SYNC_AUDIO_MASTER, /* default choice */
    AV_SYNC_VIDEO_MASTER,
    AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
};



// 待解码包队列
struct PacketQueue {
    AVFifoBuffer *pkt_list;
    int nb_packets;
    int size;
    int64_t duration;
    int abort_request;
    int serial;

    int packet_queue_put(AVPacket *pkt)
    {
        AVPacket *pkt1;
        int ret;

        pkt1 = av_packet_alloc();
        if (!pkt1) {
            av_packet_unref(pkt);
            return -1;
        }
        av_packet_move_ref(pkt1, pkt);

//        SDL_LockMutex(mutex);
        ret = packet_queue_put_private(pkt1);
//        SDL_UnlockMutex(q->mutex);

        if (ret < 0)
            av_packet_free(&pkt1);

        return ret;
    }

    int packet_queue_put_private(AVPacket *pkt)
    {
        MyAVPacketList pkt1;

        if (abort_request)
            return -1;

        if (av_fifo_space(pkt_list) < sizeof(pkt1)) {
            if (av_fifo_grow(pkt_list, sizeof(pkt1)) < 0)
                return -1;
        }

        pkt1.pkt = pkt;
        pkt1.serial = serial;

        av_fifo_generic_write(pkt_list, &pkt1, sizeof(pkt1), NULL);
        nb_packets++;
        size += pkt1.pkt->size + sizeof(pkt1);
        duration += pkt1.pkt->duration;
        /* XXX: should duplicate packet data in DV case */
//        SDL_CondSignal(q->cond);
        return 0;
    }

    int packet_queue_put_nullpacket(AVPacket *pkt, int stream_index)
    {
        pkt->stream_index = stream_index;
        return packet_queue_put(pkt);
    }

    int stream_has_enough_packets(AVStream *st, int stream_id) {
        return stream_id < 0 ||
               abort_request ||
               (st->disposition & AV_DISPOSITION_ATTACHED_PIC) ||
               nb_packets > MIN_FRAMES && (!duration || av_q2d(st->time_base) * duration > 1.0);
    }


};

struct Clock {
    double pts;           /* clock base */
    double pts_drift;     /* clock base minus time at which we updated the clock */
    double last_updated;
    double speed;
    int serial;           /* clock is based on a packet with this serial */
    int paused;
    int *queue_serial;    /* pointer to the current packet queue serial, used for obsolete clock detection */

    void set_clock(double pts, int serial)
    {
        double time = av_gettime_relative() / 1000000.0;
        this->pts = pts;
        this->last_updated = time;
        this->pts_drift = this->pts - time;
        this->serial = serial;
    }

    double get_clock()
    {
        if (*queue_serial != serial)
            return NAN;
        if (paused) {
            return pts;
        } else {
            double time = av_gettime_relative() / 1000000.0;
            return pts_drift + time - (time - last_updated) * (1.0 - speed);
        }
    }
};

struct FrameQueue {
    Frame queue[FRAME_QUEUE_SIZE];
    int rindex;
    int windex;
    int size;
    int max_size;
    int keep_last;
    int rindex_shown;
    PacketQueue *pktq;

    /* return the number of undisplayed frames in the queue */
    int frame_queue_nb_remaining()
    {
        return size - rindex_shown;
    }
};

typedef struct Decoder {
    AVPacket *pkt;
    PacketQueue *queue;
    AVCodecContext *avctx;
    int pkt_serial;
    int finished;
    int packet_pending;
//    SDL_cond *empty_queue_cond;
    int64_t start_pts;
    AVRational start_pts_tb;
    int64_t next_pts;
    AVRational next_pts_tb;
//    SDL_Thread *decoder_tid;
} Decoder;



#endif //ANDROIDTEST_FFINC_H
