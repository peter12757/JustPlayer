//
// Created by Administrator on 2024/7/28.
//

#include "Profiler.h"



Profiler::Profiler(int max_sample) {
//    memset(profiler, 0, sizeof(_Profiler));
    if (max_sample < 0)
        max_sample = 3;
    else
        max_sample = max_sample;
}

Profiler::~Profiler() {

}

void Profiler::SDL_ProfilerReset(int max_sample) {
    memset(this, 0, sizeof(Profiler));
    if (max_sample < 0)
        max_sample = 3;
    else
        max_sample = max_sample;
}

uint64_t Profiler::getTickHR(void) {
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

void Profiler::SDL_ProfilerBegin() {
    begin_time = getTickHR();
}

int64_t Profiler::SDL_ProfilerEnd() {
    int64_t delta = getTickHR() - begin_time;

    if (max_sample > 0) {
        total_elapsed += delta;
        total_counter += 1;

        sample_elapsed += delta;
        sample_counter  += 1;

        if (sample_counter > max_sample) {
            sample_elapsed -= average_elapsed;
            sample_counter -= 1;
        }

        if (sample_counter > 0) {
            average_elapsed = sample_elapsed / sample_counter;
        }
        if (sample_elapsed > 0) {
            sample_per_seconds = sample_counter * 1000.f / sample_elapsed;
        }
    }

    return delta;
}




