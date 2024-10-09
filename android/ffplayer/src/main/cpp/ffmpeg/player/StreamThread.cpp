//
// Created by Administrator on 2024/9/25.
//

#include "StreamThread.h"




StreamThread::StreamThread(VideoState *is,int stream_index,int stream_lowers)
{

}

StreamThread::~StreamThread() {

}

void StreamThread::onCreate() {


}

void StreamThread::onThreadRun(uint32_t now) {

}

void StreamThread::onStop() {

}

int StreamThread::get_master_sync_type() {
    if (mediaState->av_sync_type == AV_SYNC_VIDEO_MASTER) {
        if (mediaState->video_st)
            return AV_SYNC_VIDEO_MASTER;
        else
            return AV_SYNC_AUDIO_MASTER;
    } else if (mediaState->av_sync_type == AV_SYNC_AUDIO_MASTER) {
        if (mediaState->audio_st)
            return AV_SYNC_AUDIO_MASTER;
        else
            return AV_SYNC_EXTERNAL_CLOCK;
    } else {
        return AV_SYNC_EXTERNAL_CLOCK;
    }
    return 0;
}

double StreamThread::get_master_clock() {
    Clock *clk;

    switch (get_master_sync_type()) {
        case AV_SYNC_VIDEO_MASTER:
            clk = mediaState->vidclk;
            break;
        case AV_SYNC_AUDIO_MASTER:
            clk = mediaState->audclk;
            break;
        default:
            clk = mediaState->extclk;
            break;
    }
    return clk->get_clock();
}


