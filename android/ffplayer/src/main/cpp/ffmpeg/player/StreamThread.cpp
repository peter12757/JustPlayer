//
// Created by Administrator on 2024/9/25.
//

#include "StreamThread.h"




StreamThread::StreamThread(VideoState *is,int stream_index,int stream_lowers)
:mediaState(is)
,stream_index(stream_index)
,stream_lowers(stream_lowers) {

    if (stream_index <0 || stream_index>= mediaState->ic->nb_streams) {
        log(

                )
    }

}

StreamThread::~StreamThread() {

}

void StreamThread::onCreate() {


}

void StreamThread::onThreadRun(uint32_t now) {

}

void StreamThread::onStop() {

}