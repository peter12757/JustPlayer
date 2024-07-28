//
// Created by Administrator on 2024/7/28.
//

#ifndef JUSTPLAYER_AUDIOPARAMS_H
#define JUSTPLAYER_AUDIOPARAMS_H


#include <cstdint>
#include <samplefmt.h>

class AudioParams {
    int freq;
    int channels;
    int64_t channel_layout;
    AVSampleFormat fmt;
    int frame_size;
    int bytes_per_sec;
};


#endif //JUSTPLAYER_AUDIOPARAMS_H
