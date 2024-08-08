//
// Created by Administrator on 2024/8/5.
//

#include "VideoState.h"

#include <utility>

VideoState::VideoState(std::string url,AVInputFormat *iformat,int av_sync_type)
:datasourcel(url)
,iformat(iformat)
,ytop(0)
,xleft(0)
,pictq_size(VIDEO_PICTURE_QUEUE_SIZE_DEFAULT)
,audio_clock_serial(-1)
,audio_volume(SDL_MIX_MAXVOLUME)
,mute(0)
,av_sync_type(av_sync_type)
{

}

VideoState::~VideoState() {

}
