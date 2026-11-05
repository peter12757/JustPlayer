//
// Created by Administrator on 2026/10/27.
//

#ifndef JUSTPLAYER_FFCONTEXT_H
#define JUSTPLAYER_FFCONTEXT_H


#include <android/native_window.h>
#include <string>
#include "config/FFdef.h"
#include "VideoState.h"
#include "AudioState.h"
#include "MediaMeta.h"
#include "const/common.h"

using namespace std;

class FFContext {


public:
    ANativeWindow *native_window = nullptr;


    string mURI;
    VideoState *videoState;
    AudioState *audioState;
    MediaMeta *meta;


    FFContext(string uri);
    virtual ~FFContext();


    /* options specified by the user */
    const AVInputFormat *file_iformat;
    const char *input_filename;
    const char *window_title;
    int default_width  = 640;
    int default_height = 480;
    int screen_width  = 0;
    int screen_height = 0;
    int screen_left = 0;
    int screen_top = 0;
    int audio_disable;
    int video_disable;
    int subtitle_disable;
    const char* wanted_stream_spec[AVMEDIA_TYPE_NB] = {0};
    int seek_by_bytes = -1;
    float seek_interval = 10;
    int display_disable;
    int borderless;
    int alwaysontop;
    int startup_volume = 100;
    int show_status = -1;
    int av_sync_type = AV_SYNC_AUDIO_MASTER;
    int64_t start_time = AV_NOPTS_VALUE;
    int64_t duration = AV_NOPTS_VALUE;
    int fast = 0;
    int genpts = 0;
    int lowres = 0;
    int decoder_reorder_pts = -1;
    int autoexit;
    int exit_on_keydown;
    int exit_on_mousedown;
    int loop = 1;
    int framedrop = -1;
    int infinite_buffer = -1;
    enum ShowMode show_mode = SHOW_MODE_NONE;
    const char *audio_codec_name;
    const char *subtitle_codec_name;
    const char *video_codec_name;
    double rdftspeed = 0.02;
    int64_t cursor_last_shown;
    int cursor_hidden = 0;
    const char **vfilters_list = NULL;
    int nb_vfilters = 0;
    char *afilters = NULL;
    int autorotate = 1;
    int find_stream_info = 1;
    int filter_nbthreads = 0;
    int enable_vulkan = 0;
    char *vulkan_params = NULL;
    char *video_background = NULL;
    const char *hwaccel = NULL;

/* current context */
    int is_full_screen;
    int64_t audio_callback_time;

    const AVInputFormat *iformat{};
    int abort_request{};
    int force_refresh{};
    int paused{};
    int last_paused{};
    int queue_attachments_req{};
    int seek_req{};
    int seek_flags{};
    int64_t seek_pos{};
    int64_t seek_rel{};
    int read_pause_return{};
    AVFormatContext *ic{};
    int realtime{};

    Clock audclk{};
    Clock vidclk{};
    Clock extclk{};

    FrameQueue pictq{};
    FrameQueue subpq{};
    FrameQueue sampq{};

    Decoder auddec{};
    Decoder viddec{};
    Decoder subdec{};

    int audio_stream{};


    double audio_clock{};
    int audio_clock_serial{};
    double audio_diff_cum{}; /* used for AV difference average computation */
    double audio_diff_avg_coef{};
    double audio_diff_threshold{};
    int audio_diff_avg_count{};
    AVStream *audio_st{};
    PacketQueue audioq{};
    int audio_hw_buf_size{};
    uint8_t *audio_buf{};
    uint8_t *audio_buf1{};
    unsigned int audio_buf_size{}; /* in bytes */
    unsigned int audio_buf1_size{};
    int audio_buf_index{}; /* in bytes */
    int audio_write_buf_size{};
    int audio_volume{};
    int muted{};
    struct AudioParams audio_src{};
    struct AudioParams audio_filter_src{};
    struct AudioParams audio_tgt{};
    struct SwrContext *swr_ctx{};
    int frame_drops_early{};
    int frame_drops_late{};

    int16_t sample_array[SAMPLE_ARRAY_SIZE]{};
    int sample_array_index{};
    int last_i_start{};
    AVTXContext *rdft{};
    av_tx_fn rdft_fn{};
    int rdft_bits{};
    float *real_data{};
    AVComplexFloat *rdft_data{};
    int xpos{};
    double last_vis_time{};
//    RenderParams render_params;
//    SDL_Texture *vis_texture;
//    SDL_Texture *sub_texture;
//    SDL_Texture *vid_texture;

    int subtitle_stream{};
    AVStream *subtitle_st{};
    PacketQueue subtitleq{};

    double frame_timer{};
    double frame_last_returned_time{};
    double frame_last_filter_delay{};
    int video_stream{};
    AVStream *video_st{};
    PacketQueue videoq{};
    double max_frame_duration{};      // maximum duration of a frame - above this, we consider the jump a timestamp discontinuity
    struct SwsContext *sub_convert_ctx{};
    int eof{};



    int vfilter_idx{};
    AVFilterContext *in_video_filter{};   // the first filter in the video chain
    AVFilterContext *out_video_filter{};  // the last filter in the video chain
    AVFilterContext *in_audio_filter{};   // the first filter in the audio chain
    AVFilterContext *out_audio_filter{};  // the last filter in the audio chain
    AVFilterGraph *agraph{};              // audio filter graph

    int last_video_stream =-1, last_audio_stream=-1, last_subtitle_stream=-1;

//    SDL_cond *continue_read_thread;

};


#endif //JUSTPLAYER_FFCONTEXT_H
