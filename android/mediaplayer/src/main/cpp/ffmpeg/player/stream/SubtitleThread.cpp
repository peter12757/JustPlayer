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
