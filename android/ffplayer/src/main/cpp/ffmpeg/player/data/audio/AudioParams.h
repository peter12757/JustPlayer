//
// Created by Administrator on 2024/7/28.
//

#ifndef JUSTPLAYER_AUDIOPARAMS_H
#define JUSTPLAYER_AUDIOPARAMS_H


#include <cstdint>
#include "FFinc.h"

class AudioParams {
public:
    int freq;
    AVChannelLayout ch_layout;
    enum AVSampleFormat fmt;
    int frame_size;
    int bytes_per_sec;
};


#endif //JUSTPLAYER_AUDIOPARAMS_H
