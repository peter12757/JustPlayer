//
// Created by PeterXi on 2024/8/10.
//

#ifndef JUSTPLAYER_READTHREAD_H
#define JUSTPLAYER_READTHREAD_H

#include <vector>
#include "data/VideoState.h"
#include "XThread.h"


class ReadThread :XThread {
public:
    VideoState *mediaState;

    int errCode;
    AVDictionaryEntry *t;

    int scan_all_pmts_set = 0;






    ReadThread(VideoState *is);
    ~ReadThread();

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


    static int decode_interrupt_cb(void *ctx) {
        VideoState *is = (VideoState *)ctx;
        return is->abort_request;
    }



};


#endif //JUSTPLAYER_READTHREAD_H
