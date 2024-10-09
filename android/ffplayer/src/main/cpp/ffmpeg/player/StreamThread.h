//
// Created by Administrator on 2024/9/25.
//

#ifndef JUSTPLAYER_STREAMTHREAD_H
#define JUSTPLAYER_STREAMTHREAD_H


#include "VideoState.h"
#include "../JustLogger.h"

class StreamThread {
public:
    VideoState *mediaState;
    AVCodecContext *avctx;
    const AVCodec *codec;
    std::string forced_codec_name;
    AVDictionary *opts;
    const AVDictionaryEntry *t;
    int stream_index = -1;
    int sample_rate = -1;
//    AVChannelLayout ch_layout = {0};
    int err_code = 0;
    int stream_lowers;

    StreamThread(VideoState *is,int stream_index,int stream_lowers);
    ~StreamThread();

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

    //sync clock
    int get_master_sync_type();
    double get_master_clock();

};


#endif //JUSTPLAYER_STREAMTHREAD_H
