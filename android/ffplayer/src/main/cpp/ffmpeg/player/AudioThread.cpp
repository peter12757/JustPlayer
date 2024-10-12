//
// Created by Administrator on 2024/9/23.
//


#include "AudioThread.h"

AudioThread::AudioThread(VideoState *is,int stream_index,int stream_lowers)
        : StreamThread(is,stream_index,stream_lowers)
        {
    std::ostringstream log;
    log<<"AudioThread";
    frame = av_frame_alloc();
    ic = is->ic;
}

AudioThread::~AudioThread() {

}

void AudioThread::onCreate() {
    std::ostringstream log;
    log<<"AudioThread::onCreate";
    if (stream_index < 0 || stream_index >= ic->nb_streams) {
        log<<" error stream_index < 0 || stream_index >= ic->nb_streams";
        LOGE("%s",log.str().c_str());
    }

}

void AudioThread::onThreadRun(uint32_t now) {

}

void AudioThread::onStop() {

}
