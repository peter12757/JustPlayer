//
// Created by Administrator on 2024/8/5.
//

#ifndef JUSTPLAYER_MEDIACODECBUFFERINFO_H
#define JUSTPLAYER_MEDIACODECBUFFERINFO_H


#include <cstdint>

class MediaCodecBufferInfo {
public:
    int32_t offset;
    int32_t size;
    int64_t presentationTimeUs;
    uint32_t flags;
};


#endif //JUSTPLAYER_MEDIACODECBUFFERINFO_H
