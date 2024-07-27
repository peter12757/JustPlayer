//
// Created by Peter Xi on 2022/6/3.
//

#ifndef ANDROIDTEST_FFINC_H
#define ANDROIDTEST_FFINC_H
extern "C"{
#include "avutil.h"
#include "avcodec.h"
#include "avformat.h"
#include "avdevice.h"
#include "avfilter.h"
#include "log.h"
#include "avfft.h"
#include "fftime.h"
#include "dict.h"
#include "channel_layout.h"
#include "samplefmt.h"
#include "buffersrc.h"
#include "buffersink.h"
#include "fifo.h"
#include "swscale.h"

#include "file.h"
#include "mem.h"
#include "fifo.h"
#include "avstring.h"
#include "pixdesc.h"
#include "swresample.h"
}
#include <sstream>

#include "config/FFConfig.h"
#include "constants.h"
#include "common.h"


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

class MyAVPacketList {
public:
    AVPacket pkt;
    int serial;
    MyAVPacketList *next;
};

enum AVSyncType{
    AV_SYNC_AUDIO_MASTER, /* default choice */
    AV_SYNC_VIDEO_MASTER,
    AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
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



#endif //ANDROIDTEST_FFINC_H
