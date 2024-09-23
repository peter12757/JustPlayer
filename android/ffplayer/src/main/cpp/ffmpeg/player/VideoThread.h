//
// Created by Administrator on 2024/9/23.
//

#ifndef JUSTPLAYER_VIDEOTHREAD_H
#define JUSTPLAYER_VIDEOTHREAD_H


#include "VideoState.h"

class VideoThread {
public:
    VideoState *mediaState;


    VideoThread(VideoState *is);
    ~VideoThread();

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
