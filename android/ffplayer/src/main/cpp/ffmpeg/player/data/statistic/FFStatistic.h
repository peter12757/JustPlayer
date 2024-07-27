//
// Created by Administrator on 2024/7/22.
//

#ifndef JUSTPLAYER_FFSTATISTIC_H
#define JUSTPLAYER_FFSTATISTIC_H


#include <cstdint>
#include "../SpeedSampler2.h"

#define FFP_TCP_READ_SAMPLE_RANGE 2000

typedef struct FFTrackCacheStatistic
{
    int64_t duration;
    int64_t bytes;
    int64_t packets;
} FFTrackCacheStatistic;

class FFStatistic {
public:
    int64_t vdec_type;

    float vfps;
    float vdps;
    float avdelay;
    float avdiff;
    int64_t bit_rate;

    FFTrackCacheStatistic video_cache;
    FFTrackCacheStatistic audio_cache;

    int64_t buf_backwards;
    int64_t buf_forwards;
    int64_t buf_capacity;
    SpeedSampler2 tcp_read_sampler;
    int64_t latest_seek_load_duration;

    FFStatistic();
    ~FFStatistic();

    void resetStatistic();
};


#endif //JUSTPLAYER_FFSTATISTIC_H
