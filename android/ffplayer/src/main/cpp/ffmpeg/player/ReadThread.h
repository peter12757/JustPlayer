//
// Created by PeterXi on 2024/8/10.
//

#ifndef JUSTPLAYER_READTHREAD_H
#define JUSTPLAYER_READTHREAD_H

#include <vector>
#include "data/VideoState.h"
#include "XThread.h"
#include "FFMsg.h"
#include "FFinc.h"

class VideoState;
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


    int stream_component_open(VideoState *mediaState, int stream_index);
    void set_avformat_context(MediaMeta *meta, AVFormatContext *ic);





    //ffmpeg
    AVDictionary **setup_find_stream_info_opts(AVFormatContext *s,
                                               AVDictionary *codec_opts);
    AVDictionary *filter_codec_opts(AVDictionary *opts, enum AVCodecID codec_id,
                                    AVFormatContext *s, AVStream *st, const AVCodec *codec);

    static int check_stream_specifier(AVFormatContext *s, AVStream *st, const char *spec)
    {
        int ret = avformat_match_stream_specifier(s, st, spec);
        if (ret < 0)
            av_log(s, AV_LOG_ERROR, "Invalid stream specifier: %s.\n", spec);
        return ret;
    }



};


#endif //JUSTPLAYER_READTHREAD_H
