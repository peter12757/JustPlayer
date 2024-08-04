//
// Created by Administrator on 2024/8/5.
//

#ifndef JUSTPLAYER_VIDEOOBJ_H
#define JUSTPLAYER_VIDEOOBJ_H

#include <android/native_window_jni.h>
#include <vector>
#include "mediacodec/MediaCodecBuffer.h"

class VideoObj {

public:
    //vout
    ANativeWindow *native_window = nullptr;

    int null_native_window_warned = 1; // reduce log for null window

    std::vector<MediaCodecBuffer *> overlay_manager;
    std::vector<MediaCodecBuffer *> overlay_pool;

    VideoObj();
    ~VideoObj();

    void resetAllMediaCodecBuffers();


};


#endif //JUSTPLAYER_VIDEOOBJ_H
