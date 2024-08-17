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
#include "meta/MediaMeta.h"

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

    static int64_t get_bit_rate(AVCodecParameters *codecpar)
    {
        int64_t bit_rate;
        int bits_per_sample;

        switch (codecpar->codec_type) {
            case AVMEDIA_TYPE_VIDEO:
            case AVMEDIA_TYPE_DATA:
            case AVMEDIA_TYPE_SUBTITLE:
            case AVMEDIA_TYPE_ATTACHMENT:
                bit_rate = codecpar->bit_rate;
                break;
            case AVMEDIA_TYPE_AUDIO:
                bits_per_sample = av_get_bits_per_sample(codecpar->codec_id);
                bit_rate = bits_per_sample ? codecpar->sample_rate * codecpar->channels * bits_per_sample : codecpar->bit_rate;
                break;
            default:
                bit_rate = 0;
                break;
        }
        return bit_rate;
    }

    static int stream_has_enough_packets(AVStream *st, int stream_id, PacketQueue *queue, int min_frames) {
        return stream_id < 0 ||
               queue->abort_request ||
               (st->disposition & AV_DISPOSITION_ATTACHED_PIC) ||
               #ifdef FFP_MERGE
               queue->nb_packets > MIN_FRAMES && (!queue->duration || av_q2d(st->time_base) * queue->duration > 1.0);
               #endif
               queue->nb_packets > min_frames;
    }




};


#endif //JUSTPLAYER_READTHREAD_H
