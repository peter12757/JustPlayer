//
// Created by Administrator on 2026/11/6.
//

#ifndef JUSTPLAYER_AUDIOTHREAD_H
#define JUSTPLAYER_AUDIOTHREAD_H


#include "data/FFContext.h"
#include "thread/XThread.h"

class AudioThread :XThread{

public:
    FFContext *ctx;

    AudioThread(FFContext *pContext);
    virtual ~AudioThread();

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


#endif //JUSTPLAYER_AUDIOTHREAD_H
