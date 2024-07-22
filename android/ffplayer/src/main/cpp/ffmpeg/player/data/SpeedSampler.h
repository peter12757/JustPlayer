//
// Created by Administrator on 2024/7/22.
//

#ifndef JUSTPLAYER_SPEEDSAMPLER_H
#define JUSTPLAYER_SPEEDSAMPLER_H


#include <stdint.h>
#include <time.h>

class SpeedSampler {
public:
    uint64_t  samples[10];
    int     capacity;
    int     count;
    int     first_index;
    int     next_index;

    uint64_t  last_log_time;


    uint64_t getTickHR(void);

    void  resetSpeedSampler();
// return samples per seconds
    float addSpeedSampler(int enable_log, const char *log_tag);
};


#endif //JUSTPLAYER_SPEEDSAMPLER_H
