//
// Created by Administrator on 2024/8/5.
//

#ifndef JUSTPLAYER_VIDEOSTATE_H
#define JUSTPLAYER_VIDEOSTATE_H

#include <string>
#include "FFinc.h"
#include "FrameQueue.h"
#include "PacketQueue.h"
#include "Clock.h"
#include "data/audio/AudioObj.h"
#include "MyFFPlayer.h"

class MyFFPlayer;

class VideoState {

public:
    std::string datasourcel;

    AVInputFormat *iformat;
    std::string filename;
    int width, height, xleft, ytop;
    MyFFPlayer *player;

    //video
    FrameQueue *pictq;
    PacketQueue *videoq;
    Clock *vidclk;
    int pictq_size;
    int last_video_stream;
    int video_stream;


    //subtitle
    FrameQueue *subpq;
    PacketQueue *subtitleq;
    int last_subtitle_stream;
    int subtitle_stream;

    //audio
    FrameQueue *sampq;
    PacketQueue *audioq;
    Clock *audclk;
    int audio_clock_serial;
    int audio_volume;
    bool muted;
    int last_audio_stream;
    int audio_stream;
    AVSyncType av_sync_type;


    //read stream input
    int eof;

    AVFormatContext *ic;


    /* format/codec options */
    AVDictionary *format_opts;
    AVDictionary *codec_opts;
    AVDictionary *sws_dict;
    AVDictionary *player_opts;
    AVDictionary *swr_opts;
    AVDictionary *swr_preset_opts;

    //使用三方同步同步
    Clock *extclk;

    int genpts;//????

    //stream info
    int find_stream_info;

    bool abort_request;
    bool pause_req;


    //seek
    int seek_by_bytes;
    int64_t start_time;
    int realtime;

    double max_frame_duration;      // maximum duration of a frame - above this, we consider the jump a timestamp discontinuity

    VideoState(MyFFPlayer * player,std::string url,AVInputFormat *iformat,int av_sync_type);
    ~VideoState();

    static int decode_interrupt_cb(void *ctx) {
        VideoState *is = (VideoState *)ctx;
        return is->abort_request;
    }


    void reset();

};


#endif //JUSTPLAYER_VIDEOSTATE_H
