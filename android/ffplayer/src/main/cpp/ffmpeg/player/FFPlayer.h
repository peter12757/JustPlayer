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
#include "data/VideoState.h"
#include "LocalDataReader.h"
#include "PlayerEventHandler.h"
#include "package.h"


using namespace std;
using namespace JustPackage;

class AudioBaseEngine;

class VideoState;

class LocalDataReader;

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


class FFPlayer : public XThread {
    //data
protected:
    VideoState *mJustPlayerCtx{};


    //thread
    virtual void onThreadRun(uint32_t now);

public:
    FFPlayer();

    ~FFPlayer();


    RET setDataSource(const string &uri);

    RET prepareAsyc();

    RET setSurface(JNIEnv *env,jobject android_surface);

    RET start();

    RET pause();

    RET stop();

    RET reset();


    void changePlayerState(PlayerState state);

    RET invoke(Unpack unpack);

    void setCallBack(JNIEnv *pEnv, jobject jcallback);

    jobject callJavaMethod(string data);




    //ffmpeg api


    //thread
    virtual void onCreate();


protected:
    PlayerEventHandler *m_pHandler;


    //参考ijk的一些东西
    const AVClass *av_class;

    /* ffplay context */
    VideoState *is;

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
    enum VideoState::ShowMode show_mode;
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
    SDL_Aout *aout;
    SDL_Vout *vout;
    struct IJKFF_Pipeline *pipeline;
    struct IJKFF_Pipenode *node_vdec;
    int sar_num;
    int sar_den;

    char *video_codec_info;
    char *audio_codec_info;
    char *subtitle_codec_info;
    Uint32 overlay_format;

    int last_error;
    int prepared;
    int auto_resume;
    int error;
    int error_count;
    int start_on_prepared;
    int first_video_frame_rendered;
    int first_audio_frame_rendered;
    int sync_av_start;

    MessageQueue msg_queue;

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

    struct IjkMediaMeta *meta;

    SDL_SpeedSampler vfps_sampler;
    SDL_SpeedSampler vdps_sampler;

    /* filters */
    SDL_mutex  *vf_mutex;
    SDL_mutex  *af_mutex;
    int         vf_changed;
    int         af_changed;
    float       pf_playback_rate;
    int         pf_playback_rate_changed;
    float       pf_playback_volume;
    int         pf_playback_volume_changed;

    void               *inject_opaque;
    FFStatistic         stat;
    FFDemuxCacheControl dcc;

    AVApplicationContext *app_ctx;



};


#endif //ANDROIDTEST_JUSTPLAYER_H
