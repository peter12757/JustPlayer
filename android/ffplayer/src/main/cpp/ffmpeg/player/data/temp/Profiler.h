//
// Created by Administrator on 2024/7/28.
//

#ifndef JUSTPLAYER_PROFILER_H
#define JUSTPLAYER_PROFILER_H


#include <cstdint>
#include "FFinc.h"

class Profiler {
public:
    int64_t total_elapsed;
    int     total_counter;

    int64_t sample_elapsed;
    int     sample_counter;
    float   sample_per_seconds;
    int64_t average_elapsed;

    int64_t begin_time;

    int     max_sample;

    Profiler(int max_sample);
    ~Profiler();

    void    SDL_ProfilerReset(int max_sample);
    void    SDL_ProfilerBegin();
    int64_t SDL_ProfilerEnd();

    uint64_t getTickHR();
};


#endif //JUSTPLAYER_PROFILER_H
