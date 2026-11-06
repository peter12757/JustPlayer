//
// Created by Administrator on 2026/11/6.
//

#ifndef JUSTPLAYER_VIDEOTHREAD_H
#define JUSTPLAYER_VIDEOTHREAD_H


#include "data/FFContext.h"
#include "thread/XThread.h"

class VideoThread :XThread{

public:
    FFContext *ctx;

    VideoThread(FFContext *pContext);
    virtual ~VideoThread();


    //thread
    void onCreate();
    virtual void onThreadRun(uint32_t now);
    void onStop();

    void stopThread();
    void startThread();
    void wakeUp();
    void resetInterval(uint32_t interval);
    bool isQuit() const;
    void resetWakeUpEvent();
    void setMode(ThreadMode mode);
};


#endif //JUSTPLAYER_VIDEOTHREAD_H
