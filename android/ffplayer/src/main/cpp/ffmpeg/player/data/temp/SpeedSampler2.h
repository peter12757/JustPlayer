//
// Created by Administrator on 2024/7/22.
//

#ifndef JUSTPLAYER_SPEEDSAMPLER2_H
#define JUSTPLAYER_SPEEDSAMPLER2_H

#include <cstdint>
#include <cstring>
#include <ctime>
#include <bits/timespec.h>

//带宽预测
class SpeedSampler2 {
public:
    int64_t sample_range;
    int64_t last_profile_tick;
    int64_t last_profile_duration;
    int64_t last_profile_quantity;
    int64_t last_profile_speed;

    uint64_t getTickHR(void);

    void    resetSpeedSampler(int sample_range);
    int64_t addSpeedSampler(int quantity);
    int64_t getSpeedSpeedSampler();
};


#endif //JUSTPLAYER_SPEEDSAMPLER2_H
