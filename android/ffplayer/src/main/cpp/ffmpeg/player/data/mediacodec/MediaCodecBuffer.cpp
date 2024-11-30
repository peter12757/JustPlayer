//
// Created by Administrator on 2024/8/5.
//

#include "MediaCodecBuffer.h"

MediaCodecBuffer::MediaCodecBuffer()
:buffer_id(0)
,buffer_index(-1)
,acodec_serial(0){
    buffer_info = new MediaCodecBufferInfo();
}

MediaCodecBuffer::~MediaCodecBuffer() {
    reset();
}

void MediaCodecBuffer::reset() {
    buffer_index  = -1;
    acodec_serial = 0;
    SafeDelete(buffer_info);
}
