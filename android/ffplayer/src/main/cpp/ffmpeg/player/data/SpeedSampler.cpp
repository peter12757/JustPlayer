//
// Created by Administrator on 2024/7/22.
//

#include <string.h>
#include <bits/timespec.h>
#include "SpeedSampler.h"
#include "../../../base/log/LogUtil.h"

void SpeedSampler::resetSpeedSampler() {
    memset(this,0, sizeof(SpeedSampler));
    capacity = sizeof(samples) / sizeof(uint64_t);
}

float SpeedSampler::addSpeedSampler(int enable_log, const char *log_tag) {
    uint64_t current = getTickHR();
    samples[next_index] = current;
    next_index++;
    next_index %= capacity;
    if (count + 1 >= capacity) {
        first_index++;
        first_index %= capacity;
    } else {
        count++;
    }

    if (count < 2)
        return 0;

    float samples_per_second = 1000.0f * (count - 1) / (current - samples[first_index]);

    if (enable_log && (last_log_time + 1000 < current || last_log_time > current)) {
        last_log_time = current;
        LOGW("%s: %.2f\n", log_tag ? log_tag : "N/A", samples_per_second);
    }

    return samples_per_second;
}

uint64_t SpeedSampler::getTickHR(void) {
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


