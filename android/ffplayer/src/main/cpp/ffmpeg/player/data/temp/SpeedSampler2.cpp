//
// Created by Administrator on 2024/7/22.
//



#include "SpeedSampler2.h"


void SpeedSampler2::resetSpeedSampler(int sample_range) {
    memset(this, 0, sizeof(SpeedSampler2));
    sample_range      = sample_range;
    last_profile_tick = (int64_t)getTickHR();

}

uint64_t SpeedSampler2::getTickHR(void) {
    uint64_t clock;
#if defined(__ANDROID__)
    struct timespec now;
#ifdef CLOCK_MONOTONIC_COARSE
    clock_gettime(CLOCK_MONOTONIC_COARSE, &now);
#else
    clock_gettime(CLOCK_MONOTONIC_HR, &now);
#endif
    clock = now.tv_sec * 1000 + now.tv_nsec / 1000000;
#elif defined(__APPLE__)
    if (!g_is_mach_base_info_inited) {
        g_mach_base_info_ret = mach_timebase_info(&g_mach_base_info);
        g_is_mach_base_info_inited = 1;
    }
    if (g_mach_base_info_ret == 0) {
        uint64_t now = mach_absolute_time();
        clock = now * g_mach_base_info.numer / g_mach_base_info.denom / 1000000;
    } else {
        struct timeval now;
        gettimeofday(&now, NULL);
        clock = now.tv_sec  * 1000 + now.tv_usec / 1000;
    }
#endif
    return (clock);

}
