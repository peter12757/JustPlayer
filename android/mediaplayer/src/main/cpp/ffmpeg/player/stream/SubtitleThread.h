//
// Created by Administrator on 2026/11/6.
//

#ifndef JUSTPLAYER_SUBTITLETHREAD_H
#define JUSTPLAYER_SUBTITLETHREAD_H


#include "data/FFContext.h"
#include "thread/XThread.h"

class SubtitleThread :XThread{

public:
    FFContext *ctx;

    SubtitleThread(FFContext *pContext);
    ~SubtitleThread();


    //thread
    void onCreate() override;
    virtual void onThreadRun(uint32_t now) override;
    void onStop() override;

    void stopThread();
    void startThread();
    void wakeUp();
    void resetInterval(uint32_t interval);
    bool isQuit() const;
    void resetWakeUpEvent();
    void setMode(ThreadMode mode);
};


#endif //JUSTPLAYER_SUBTITLETHREAD_H
