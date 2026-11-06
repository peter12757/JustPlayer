//
// Created by Administrator on 2026/11/6.
//

#include "AudioThread.h"

AudioThread::AudioThread(FFContext *pContext):
XThread("AudioThread", 50, TIMER),
ctx(pContext){

}

AudioThread::~AudioThread() {

}
