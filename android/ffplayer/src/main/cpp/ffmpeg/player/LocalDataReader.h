//
// Created by Peter Xi on 2022/6/3.
//

#ifndef ANDROIDTEST_LOCALDATAREADER_H
#define ANDROIDTEST_LOCALDATAREADER_H

#include "../../base/thread/XThread.h"
#include "data/FFinc.h"
#include <stdio.h>
#include "data/MediaState.h"



class MediaState;

class LocalDataReader : public XThread {

public:
    LocalDataReader(MediaState *is);
    virtual ~LocalDataReader();

    void start();


private:
    //thread
    virtual void onCreate();
    virtual void onStop();
    void onThreadRun(uint32_t now) override;


public:
    //ffmpeg
    static int decode_interrupt_cb(void *ctx);
    AVDictionary *filter_codec_opts(AVDictionary *opts, enum AVCodecID codec_id,
                                    AVFormatContext *s, AVStream *st, const AVCodec *codec);

    void packet_queue_flush(PacketQueue *q);
public:
    MediaState *is;
    AVFormatContext *ic;
    AVPacket *pkt;
    const AVDictionaryEntry *t{};
    MediaState *ctx{};

    AVDictionary *format_opts{}, *codec_opts{};
    int64_t start_time = AV_NOPTS_VALUE;
    int infinite_buffer = -1;   //don't limit the input buffer size (useful with realtime streams)"
    int64_t stream_start_time{};
    int64_t pkt_ts{};
    int pkt_in_play_range = 0;
    int64_t duration = AV_NOPTS_VALUE;
    int scan_all_pmts_set = 0;
    int st_index[AVMEDIA_TYPE_NB]{};


};


#endif //ANDROIDTEST_LOCALDATAREADER_H
