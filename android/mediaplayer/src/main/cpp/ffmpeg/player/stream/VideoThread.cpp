//
// Created by Administrator on 2026/11/6.
//

#include "VideoThread.h"

VideoThread::VideoThread(FFContext *pContext):
XThread("VideoThread", 50, TIMER),
ctx(pContext){

}
