//
// Created by Administrator on 2026/11/4.
//

#ifndef JUSTPLAYER_SOURCEREADER_H
#define JUSTPLAYER_SOURCEREADER_H


#include "data/FFContext.h"
#include "config/FFdef.h"
#include "thread/XThread.h"
#include "config/FFConfig.h"


class SourceReader :XThread {
public:
    SourceReader(FFContext *ctx);
    virtual ~SourceReader();


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

    //callback
    static int decode_interrupt_cb(void *ctx);


    FFContext *ctx;
    AVFormatContext *ic = nullptr;
    array<int,AVMEDIA_TYPE_NB> st_index = {};
    int err,i,ret;
    AVPacket *pkt = nullptr;
    int64_t streaam_start_time;
    char metadata_descrition[96];
    int pkt_in_play_range = 0;
    const AVDictionaryEntry *t;
//    SDL_mutex *wait_mutex = SDL_CreateMutex();
    int scan_all_pmts_set = 0;
    int64_t pkt_ts;


};


#endif //JUSTPLAYER_SOURCEREADER_H
