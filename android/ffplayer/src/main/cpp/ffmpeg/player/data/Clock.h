//
// Created by Administrator on 2024/8/8.
//

#ifndef JUSTPLAYER_CLOCK_H
#define JUSTPLAYER_CLOCK_H

#include "FFinc.h"


class Clock {
public:
    double pts;           /* clock base */
    double pts_drift;     /* clock base minus time at which we updated the clock */
    double last_updated;
    double speed;
    int serial;           /* clock is based on a packet with this serial */
    bool paused;
    int *queue_serial;    /* pointer to the current packet queue serial, used for obsolete clock detection */

    Clock(int *queue_serial);
    ~Clock();

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


#endif //JUSTPLAYER_CLOCK_H
