//
// Created by Administrator on 2024/8/5.
//

#ifndef JUSTPLAYER_MEDIACODECBUFFER_H
#define JUSTPLAYER_MEDIACODECBUFFER_H

#include "MediaCodecBufferInfo.h"
#include "../../../base/const/common.h"

class MediaCodecBuffer {
public:
    int buffer_id;
    int buffer_index;
    int acodec_serial;
    MediaCodecBufferInfo *buffer_info;

    MediaCodecBuffer();
    ~MediaCodecBuffer();

    void reset();

};


#endif //JUSTPLAYER_MEDIACODECBUFFER_H
