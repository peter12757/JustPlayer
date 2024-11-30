//
// Created by Administrator on 2024/8/5.
//

#include "VideoObj.h"

VideoObj::VideoObj() {

}

VideoObj::~VideoObj() {

}

void VideoObj::resetAllMediaCodecBuffers() {
    for (auto item = std::begin(overlay_manager);  item != std::end(overlay_manager);++item) {
        (*item)->reset();
    }
}
