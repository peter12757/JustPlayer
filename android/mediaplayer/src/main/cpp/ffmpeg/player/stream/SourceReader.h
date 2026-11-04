//
// Created by Administrator on 2026/11/4.
//

#ifndef JUSTPLAYER_SOURCEREADER_H
#define JUSTPLAYER_SOURCEREADER_H


#include "../data/FFContext.h"
#include "FFdef.h"
#include "../ffmpeginc/libavformat/avformat.h"

class SourceReader {
public:
    SourceReader(FFContext *ctx);
    virtual ~SourceReader();

    FFContext *ctx;
//    AVFormatContext *ic = nullptr;
    int st_index[AVMEDIA_TYPE_NB];
    int err,i,ret;
    AvPacket *pkt = nullptr;
    int64_t streaam_start_time;
    char metadata_descrition[96];
    int pkt_in_play_range = 0;
    const AVDictionaryEntry *t;
//    SDL_mutex *wait_mutex = SDL_CreateMutex();
    int scan_all_pmts_set = 0;
    int64_t pkt_ts;


};


#endif //JUSTPLAYER_SOURCEREADER_H
