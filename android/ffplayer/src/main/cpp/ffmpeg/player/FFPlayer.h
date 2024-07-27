//
// Created by Peter Xi on 2022/5/10.
//

#ifndef ANDROIDTEST_JUSTPLAYER_H
#define ANDROIDTEST_JUSTPLAYER_H

#include "../../base/log/LogUtil.h"
#include "../../base/const/constants.h"
#include "../../base/protocol/package.h"
#include "../../JustData.h"
#include <sstream>
#include <jni.h>
#include <android/native_window_jni.h>
#include "../../base/statemachine/StateMachine.h"

#include "../../base/util/Util.h"
#include "data/MediaState.h"
#include "LocalDataReader.h"
#include "PlayerEventHandler.h"
#include "package.h"
#include "data/MessageQueue.h"
#include "data/SpeedSampler.h"
#include "data/statistic/FFStatistic.h"
#include "data/FFDemuxCacheControl.h"
#include "FFinc.h"
#include "config/FFConfig.h"
#include "config/FFPlayerOptions.h"
#include "data/meta/MediaMeta.h"
#include "data/PacketQueue.h"


using namespace std;
using namespace JustPackage;

class AudioBaseEngine;

class MediaState;

class LocalDataReader;

const AVClass FFPlayerContextClass = {
        .class_name       = "FFPlayer",
        .item_name        = "FFPlayer",
        .option           = FFPlayerOptions,
        .version          = LIBAVUTIL_VERSION_INT,
        .child_next       = NULL,
        .child_class_next = NULL,
};

enum PlayerState {
    IDLE,
    INIT,
    PREPARING,
    PREPARED,
    START,
    PAUSE,
    STOPPING,
    STOPPED,
};


static AVPacket *flush_pkt;
static bool gInited = false;
typedef int (*ff_inject_callback)(void *opaque, int type, void *data, size_t data_size);
static ff_inject_callback gFFInjectCallback;

class FFPlayer : public XThread {

    //data
protected:
    MediaState *mJustPlayerCtx{};


    //thread
    virtual void onThreadRun(uint32_t now);

public:
    FFPlayer();

    ~FFPlayer();

protected:

    //参考ijk的一些东西
    const AVClass *av_class;

    /* ffplay context */
    MediaState *is;

    /* format/codec options */
    AVDictionary *format_opts;
    AVDictionary *codec_opts;
    AVDictionary *sws_dict;
    AVDictionary *player_opts;
    AVDictionary *swr_opts;
    AVDictionary *swr_preset_opts;

    /* ffplay options specified by the user */
#ifdef FFP_MERGE
    AVInputFormat *file_iformat;
#endif
    char *input_filename;
#ifdef FFP_MERGE
    const char *window_title;
    int fs_screen_width;
    int fs_screen_height;
    int default_width;
    int default_height;
    int screen_width;
    int screen_height;
#endif
    int audio_disable;
    int video_disable;
    int subtitle_disable;
    const char* wanted_stream_spec[AVMEDIA_TYPE_NB];
    int seek_by_bytes;
    int display_disable;
    int show_status;
    int av_sync_type;
    int64_t start_time;
    int64_t duration;
    int fast;
    int genpts;
    int lowres;
    int decoder_reorder_pts;
    int autoexit;
#ifdef FFP_MERGE
    int exit_on_keydown;
    int exit_on_mousedown;
#endif
    int loop;
    int framedrop;
    int64_t seek_at_start;
    int subtitle;
    int infinite_buffer;
    enum MediaState::ShowMode show_mode;
    char *audio_codec_name;
    char *subtitle_codec_name;
    char *video_codec_name;
    double rdftspeed;
#ifdef FFP_MERGE
    int64_t cursor_last_shown;
    int cursor_hidden;
#endif
#if CONFIG_AVFILTER
    const char **vfilters_list;
    int nb_vfilters;
    char *afilters;
    char *vfilter0;
#endif
    int autorotate;

    unsigned sws_flags;

    /* current context */
#ifdef FFP_MERGE
    int is_full_screen;
#endif
    int64_t audio_callback_time;
#ifdef FFP_MERGE
    SDL_Surface *screen;
#endif

    /* extra fields */
    Aout *aout;
    Vout *vout;
    struct IJKFF_Pipeline *pipeline;
    struct IJKFF_Pipenode *node_vdec;
    int sar_num;
    int sar_den;

    char *video_codec_info;
    char *audio_codec_info;
    char *subtitle_codec_info;
    uint32_t overlay_format;

    int last_error;
    int prepared;
    int auto_resume;
    int error;
    int error_count;
    int start_on_prepared;
    int first_video_frame_rendered;
    int first_audio_frame_rendered;
    int sync_av_start;

    class MessageQueue *msg_queue;

    int64_t playable_duration_ms;

    int packet_buffering;
    int pictq_size;
    int max_fps;

    int videotoolbox;
    int vtb_max_frame_width;
    int vtb_async;
    int vtb_wait_async;
    int vtb_handle_resolution_change;

    int mediacodec_all_videos;
    int mediacodec_avc;
    int mediacodec_hevc;
    int mediacodec_mpeg2;
    int mediacodec_mpeg4;
    int mediacodec_handle_resolution_change;
    int mediacodec_auto_rotate;

    int opensles;

    char *iformat_name;

    int no_time_adjust;
    double preset_5_1_center_mix_level;

    MediaMeta *meta;

    SpeedSampler vfps_sampler;
    SpeedSampler vdps_sampler;

    /* filters */
//    SDL_mutex  *vf_mutex;
//    SDL_mutex  *af_mutex;
    int         vf_changed;
    int         af_changed;
    float       pf_playback_rate;
    int         pf_playback_rate_changed;
    float       pf_playback_volume;
    int         pf_playback_volume_changed;

    void               *inject_opaque;
    class FFStatistic         *stat;
    class FFDemuxCacheControl dcc;

//    AVApplicationContext *app_ctx;

    PlayerEventHandler *m_pHandler;

public:

    //init
    static void gInit();
    static void      gUninit();

    //log
    static void      gSetFFLogReport(int use_report);
    static void      gSetFFLogLevel(int log_level);
    static void      gSetInjectCallback(ff_inject_callback cb);

    static int gCallback(void *opaque, int type, void *data, size_t data_size) {
        if (gFFInjectCallback)
            return gFFInjectCallback(opaque, type, data, data_size);
        return 0;
    };

    //api
    void resetInternal();

    void stream_close();
    void stream_component_close(int stream_index);
    void decoder_abort(Decoder *d, FrameQueue *fq);
    void decoder_destroy(Decoder *d);
    void      ffp_reset();

/* set options before ffp_prepare_async_l() */

    void     *ffp_set_inject_opaque(void *opaque);
    void      ffp_set_option(int opt_category, const char *name, const char *value);
    void      ffp_set_option_int(int opt_category, const char *name, int64_t value);

    int       ffp_get_video_codec_info(char **codec_info);
    int       ffp_get_audio_codec_info(char **codec_info);

/* playback controll */
    int       ffp_prepare_async_l(const char *file_name);
    int       ffp_start_from_l(long msec);
    int       ffp_start_l();
    int       ffp_pause_l();
    int       ffp_is_paused_l();
    int       ffp_stop_l();
    int       ffp_wait_stop_l();

/* all in milliseconds */
    int       ffp_seek_to_l(long msec);
    long      ffp_get_current_position_l();
    long      ffp_get_duration_l();
    long      ffp_get_playable_duration_l();
    void      ffp_set_loop(int loop);
    int       ffp_get_loop();

/* for internal usage */
    int       ffp_packet_queue_init(PacketQueue *q);
    void      ffp_packet_queue_abort(PacketQueue *q);
    void      ffp_packet_queue_start(PacketQueue *q);
    void      ffp_packet_queue_flush(PacketQueue *q);
    int       ffp_packet_queue_get(PacketQueue *q, AVPacket *pkt, int block, int *serial);
    int       ffp_packet_queue_get_or_buffering(PacketQueue *q, AVPacket *pkt, int *serial, int *finished);
    int       ffp_packet_queue_put(PacketQueue *q, AVPacket *pkt);
    bool      ffp_is_flush_packet(AVPacket *pkt);

    Frame    *ffp_frame_queue_peek_writable(FrameQueue *f);
    void      ffp_frame_queue_push(FrameQueue *f);

    int       ffp_queue_picture(AVFrame *src_frame, double pts, double duration, int64_t pos, int serial);

    int       ffp_get_master_sync_type(VideoState *is);
    double    ffp_get_master_clock(VideoState *is);

    void      ffp_toggle_buffering_l(int start_buffering);
    void      ffp_toggle_buffering(int start_buffering);
    void      ffp_check_buffering_l();
    void      ffp_track_statistic_l(AVStream *st, PacketQueue *q, FFTrackCacheStatistic *cache);
    void      ffp_audio_statistic_l();
    void      ffp_video_statistic_l();
    void      ffp_statistic_l();

    int       ffp_video_thread();

    void      ffp_set_video_codec_info(const char *module, const char *codec);
    void      ffp_set_audio_codec_info(const char *module, const char *codec);
    void      ffp_set_subtitle_codec_info(const char *module, const char *codec);

    void      ffp_set_playback_rate(float rate);
    void      ffp_set_playback_volume(float volume);
    int       ffp_get_video_rotate_degrees();
    int       ffp_set_stream_selected(int stream, int selected);

    float     ffp_get_property_float(int id, float default_value);
    void      ffp_set_property_float(int id, float value);
    int64_t   ffp_get_property_int64(int id, int64_t default_value);
    void      ffp_set_property_int64(int id, int64_t value);

// must be freed with free();
    struct IjkMediaMeta *ffp_get_meta_l();

};



#endif //ANDROIDTEST_JUSTPLAYER_H
