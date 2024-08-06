//
// Created by Peter Xi on 2022/5/25.
//

#ifndef ANDROIDTEST_VIDEOSTATE_H
#define ANDROIDTEST_VIDEOSTATE_H

#include <string>
#include "FFinc.h"
#include "Vout.h"
#include "data/temp/Aout.h"
#include "SubtitleObj.h"
#include "constants.h"
#include <sstream>
#include "Util.h"
#include "data/temp/Decoder.h"


/* NOTE: the size must be big enough to compensate the hardware audio buffersize size */
/* TODO: We assume that a decoded and resampled frame fits into this buffer */
#define SAMPLE_ARRAY_SIZE (8 * 65536)
/**
 * 一些ff相关的状态和数据。
 */
class MediaState {
public:
    MediaState(std::string uri, double lastTime = 0);

    ~MediaState();

public:

//    SDL_Thread *read_tid;
//    SDL_Thread _read_tid;
    AVInputFormat *iformat;
    int abort_request;
    int force_refresh;
    int paused;
    int last_paused;
    int queue_attachments_req;
    int seek_req;
    int seek_flags;
    int64_t seek_pos;
    int64_t seek_rel;
#ifdef FFP_MERGE
    int read_pause_return;
#endif
    AVFormatContext *ic;
    int realtime;

    Clock audclk;
    Clock vidclk;
    Clock extclk;

    FrameQueue *pictq;
    FrameQueue *subpq;
    FrameQueue *sampq;

    Decoder auddec;
    Decoder viddec;
    Decoder subdec;

    int audio_stream;

    int av_sync_type;

    double audio_clock;
    int audio_clock_serial;
    double audio_diff_cum; /* used for AV difference average computation */
    double audio_diff_avg_coef;
    double audio_diff_threshold;
    int audio_diff_avg_count;
    AVStream *audio_st;
    PacketQueue *audioq;
    int audio_hw_buf_size;
    uint8_t *audio_buf;
    uint8_t *audio_buf1;
    unsigned int audio_buf_size; /* in bytes */
    unsigned int audio_buf1_size;
    int audio_buf_index; /* in bytes */
    int audio_write_buf_size;
    int audio_volume;
    int muted;
    struct AudioParams audio_src;
#if CONFIG_AVFILTER
    struct AudioParams audio_filter_src;
#endif
    struct AudioParams audio_tgt;
    struct SwrContext *swr_ctx;
    int frame_drops_early;
    int frame_drops_late;
    int continuous_frame_drops_early;

    enum ShowMode {
        SHOW_MODE_NONE = -1, SHOW_MODE_VIDEO = 0, SHOW_MODE_WAVES, SHOW_MODE_RDFT, SHOW_MODE_NB
    } show_mode;
    int16_t sample_array[SAMPLE_ARRAY_SIZE];
    int sample_array_index;
    int last_i_start;
#ifdef FFP_MERGE
    RDFTContext *rdft;
    int rdft_bits;
    FFTSample *rdft_data;
    int xpos;
#endif
    double last_vis_time;
#ifdef FFP_MERGE
    SDL_Texture *vis_texture;
    SDL_Texture *sub_texture;
#endif

    int subtitle_stream;
    AVStream *subtitle_st;
    PacketQueue *subtitleq;

    double frame_timer;
    double frame_last_returned_time;
    double frame_last_filter_delay;
    int video_stream;
    AVStream *video_st;
    PacketQueue *videoq;
    double max_frame_duration;      // maximum duration of a frame - above this, we consider the jump a timestamp discontinuity
    struct SwsContext *img_convert_ctx;
#ifdef FFP_SUB
    struct SwsContext *sub_convert_ctx;
#endif
    int eof;

    string filename;
    int width, height, xleft, ytop;
    int step;

#if CONFIG_AVFILTER
    int vfilter_idx;
    AVFilterContext *in_video_filter;   // the first filter in the video chain
    AVFilterContext *out_video_filter;  // the last filter in the video chain
    AVFilterContext *in_audio_filter;   // the first filter in the audio chain
    AVFilterContext *out_audio_filter;  // the last filter in the audio chain
    AVFilterGraph *agraph;              // audio filter graph
#endif

    int last_video_stream, last_audio_stream, last_subtitle_stream;

//    SDL_cond *continue_read_thread;
//
//    /* extra fields */
//    SDL_mutex  *play_mutex; // only guard state, do not block any long operation
//    SDL_Thread *video_refresh_tid;
//    SDL_Thread _video_refresh_tid;

    int buffering_on;
    int pause_req;

    int dropping_frame;
    int is_video_high_fps; // above 30fps
    int is_video_high_res; // above 1080p

    PacketQueue *buffer_indicator_queue;

    volatile int latest_seek_load_serial;
    volatile int64_t latest_seek_load_start_at;
};


#endif //ANDROIDTEST_VIDEOSTATE_H
