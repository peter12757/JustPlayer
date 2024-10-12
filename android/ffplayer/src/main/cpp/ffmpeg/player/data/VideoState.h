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
#include "meta/MediaMeta.h"
#include "statistic/FFStatistic.h"
#include "FFPipenode.h"
#include "FFDemuxCacheControl.h"
#include "Decoder.h"

class MyFFPlayer;
class FFStatistic;

enum ShowMode {
    SHOW_MODE_NONE = -1, SHOW_MODE_VIDEO = 0, SHOW_MODE_WAVES, SHOW_MODE_RDFT, SHOW_MODE_NB
};

#define AV_PKT_FLAG_DISCONTINUITY 0x0100

class VideoState {

public:
    std::string datasourcel;


    //config
    bool show_status;
    bool delay_inited;
    bool start_on_prepared;
    bool auto_resume;
    bool paused;
    int loop; //loop number
    bool autoexit;
    int error;
    int64_t duration;
    const char *hwaccel = NULL;
    int autorotate = 1;

    int completed = 0;

    AVInputFormat *iformat;
    std::string filename;
    int width, height, xleft, ytop;
    MyFFPlayer *player;
    const char* wanted_stream_spec[AVMEDIA_TYPE_NB];
    MediaMeta *meta;
    int queue_attachments_req;

    bool fast = false;

    //buffer???
    PacketQueue *buffer_indicator_queue;
    int infinite_buffer;
    bool packet_buffering;
    bool buffering_on;

    //seek
    int seek_by_bytes;
    int64_t start_time;
    int realtime;
    bool prepared;
    long seek_at_start;
    bool seek_req;
    int seek_flags;
    volatile int64_t latest_seek_load_start_at;
    volatile int64_t latest_seek_load_serial;   //todo???
    int64_t seek_pos;
    int64_t seek_rel;

    double max_frame_duration;      // maximum duration of a frame - above this, we consider the jump a timestamp discontinuity
    enum ShowMode show_mode;

    //使用三方同步同步
    Clock *extclk;
    int decoder_reorder_pts = -1;

    int genpts;//????

    //demux
    FFDemuxCacheControl dcc;

    //stream info
    int find_stream_info;

    bool abort_request;
    bool pause_req;

    //video
    FrameQueue *video_fq;
    PacketQueue *videoq;
    Clock *vidclk;
    int pictq_size;
    int last_video_stream;
    int video_stream;
    bool video_disable;
    AVStream *video_st;
    FFPipenode *node_vdec;
    Decoder *viddec;
    std::string video_codec_name;
    int vfilter_idx;
    AVFilterContext *in_video_filter;   // the first filter in the video chain
    AVFilterContext *out_video_filter;  // the last filter in the video chain


    //subtitle
    FrameQueue *subpq;
    PacketQueue *subtitleq;
    int last_subtitle_stream;
    int subtitle_stream;
    bool subtitle_disable;
    AVStream *subtitle_st;
    Decoder *subdec;

    //audio
    FrameQueue *audio_fq;
    PacketQueue *audioq;
    Clock *audclk;
    int audio_clock_serial;
    int audio_volume;
    bool muted;
    int last_audio_stream;
    int audio_stream;
    AVSyncType av_sync_type;
    bool audio_disable;
    AVStream *audio_st;
    Decoder *auddec;
    int volume;
    AVFilterContext *in_audio_filter;   // the first filter in the audio chain
    AVFilterContext *out_audio_filter;  // the last filter in the audio chain


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


    FFStatistic *stat;

    double frame_last_filter_delay;
    int frame_drops_early;
    int filter_nbthreads = 0;   //number of filter threads per graph
    char **vfilters_list = NULL;


    VideoState(MyFFPlayer * player,std::string url,AVInputFormat *iformat,int av_sync_type);
    ~VideoState();

    static int decode_interrupt_cb(void *ctx) {
        VideoState *is = (VideoState *)ctx;
        return is->abort_request;
    }

    int is_realtime()
    {
        if(!strcmp(ic->iformat->name, "rtp")
           || !strcmp(ic->iformat->name, "rtsp")
           || !strcmp(ic->iformat->name, "sdp")) {
            return 1;
        }
        if(ic->pb && (!strncmp(filename.c_str(), "rtp:", 4)
                          || !strncmp(filename.c_str(), "udp:", 4))) {
            return 1;
        }

        return 0;
    }

    static const char *get_error_string(int error) {
        switch (error) {
            case AVERROR(ENOMEM):       return "AVERROR(ENOMEM)";       // 12
            case AVERROR(EINVAL):       return "AVERROR(EINVAL)";       // 22
            case AVERROR(EAGAIN):       return "AVERROR(EAGAIN)";       // 35
            case AVERROR(ETIMEDOUT):    return "AVERROR(ETIMEDOUT)";    // 60
            case AVERROR_EOF:           return "AVERROR_EOF";
            case AVERROR_EXIT:          return "AVERROR_EXIT";
        }
        return "unknown";
    }


    void reset();



};


#endif //JUSTPLAYER_VIDEOSTATE_H
