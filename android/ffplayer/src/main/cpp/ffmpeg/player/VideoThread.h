//
// Created by Administrator on 2024/9/23.
//

#ifndef JUSTPLAYER_VIDEOTHREAD_H
#define JUSTPLAYER_VIDEOTHREAD_H


#include "VideoState.h"
#include "StreamThread.h"

class VideoThread: StreamThread {
public:


    VideoThread(VideoState *is,int stream_index,int stream_lowers);
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


    //video
    int create_hwaccel(AVBufferRef **device_ctx);

};


#endif //JUSTPLAYER_VIDEOTHREAD_H
