//
// Created by Administrator on 2024/9/23.
//

#ifndef JUSTPLAYER_AUDIOTHREAD_H
#define JUSTPLAYER_AUDIOTHREAD_H


#include "VideoState.h"
#include "StreamThread.h"
#include "FFinc.h"
#include <frame.h>

class AudioThread:  StreamThread{

public:
    AVFrame* frame;
    int framedrop =-1;  //由于cpu太慢等导致需要的丢帧数

    //video thread variable
    int last_w = 0;
    int last_h = 0;
    enum AVPixelFormat last_format = AV_PIX_FMT_NONE;
    int last_serial = -1;
    int last_vfilter_idx = 0;

    AVFilterGraph *graph = nullptr;
    AVFilterContext *filt_out = NULL, *filt_in = NULL;
    double pts;
    double duration;
    AVRational tb = mediaState->video_st->time_base;
    AVRational frame_rate = av_guess_frame_rate(mediaState->ic, mediaState->video_st, NULL);


    AudioThread(VideoState *is,int stream_index,int stream_lowers);
    ~AudioThread();

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
