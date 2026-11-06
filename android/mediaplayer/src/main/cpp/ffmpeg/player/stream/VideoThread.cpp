//
// Created by Administrator on 2026/11/6.
//

#include "VideoThread.h"

VideoThread::VideoThread(FFContext *pContext):
XThread("VideoThread", 50, TIMER),
ctx(pContext){

}

VideoThread::~VideoThread() {

}

void VideoThread::onCreate() {
    XThread::onCreate();
}

void VideoThread::onThreadRun(uint32_t now) {

}

void VideoThread::onStop() {
    XThread::onStop();
}
