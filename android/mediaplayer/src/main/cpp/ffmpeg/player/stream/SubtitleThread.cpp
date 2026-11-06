//
// Created by Administrator on 2026/11/6.
//

#include "SubtitleThread.h"

SubtitleThread::SubtitleThread(FFContext *pContext) :
XThread("SubtitleThread", 50, TIMER),
ctx(pContext){

}

SubtitleThread::~SubtitleThread() {

}

void SubtitleThread::onCreate() {
    XThread::onCreate();
}

void SubtitleThread::onThreadRun(uint32_t now) {

}

void SubtitleThread::onStop() {
    XThread::onStop();
}
