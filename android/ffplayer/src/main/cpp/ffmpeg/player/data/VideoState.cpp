//
// Created by Administrator on 2024/8/5.
//

#include "VideoState.h"

#include <utility>

VideoState::VideoState(std::string url,AVInputFormat *iformat)
:datasourcel(url)
,iformat(iformat)
,ytop(0)
,xleft(0)
,pictq_size(VIDEO_PICTURE_QUEUE_SIZE_DEFAULT){

}

VideoState::~VideoState() {

}
