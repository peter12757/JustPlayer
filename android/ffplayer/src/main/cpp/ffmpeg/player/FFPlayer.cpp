//
// Created by Peter Xi on 2022/5/10.
//


#include "FFPlayer.h"
#include "JniHelper.h"

using namespace std;

FFPlayer::FFPlayer()
: XThread("FFPlayer",100,URGENT){
    av_log(NULL, AV_LOG_INFO, "av_version_info: %s\n", av_version_info());
    m_pHandler = new PlayerEventHandler();

    //msg_queue_init
    memset(msg_queue, 0, sizeof(MessageQueue));
//    msg_queue->mutex = SDL_CreateMutex();
//    msg_queue->cond = SDL_CreateCond();
    msg_queue->abort_request = 1;

//    af_mutex = SDL_CreateMutex();
//    vf_mutex = SDL_CreateMutex();
    resetInternal()

    av_class = &ffp_context_class;
    meta = ijkmeta_create();

    av_opt_set_defaults(ffp);

}

FFPlayer::~FFPlayer() {
//    ffmpeg_cleanup
    SafeDelete(mJustPlayerCtx);
    SafeDelete(m_pHandler);
}

void FFPlayer::gInit() {
    if (gInited)
        return;

#if CONFIG_AVDEVICE
    avdevice_register_all();
#endif
    avformat_network_init();

    flush_pkt = av_packet_alloc();
    flush_pkt->data = (uint8_t *)&flush_pkt;
    gInited = true;
}

void FFPlayer::gUninit() {
    if (!gInited)
        return;
    // FFP_MERGE: uninit_opts
    avformat_network_deinit();

    gInited = false;
}

static int log_level_av_to_ijk(int av_level)
{
    int ijk_level = IJK_LOG_VERBOSE;
    if      (av_level <= AV_LOG_PANIC)      ijk_level = IJK_LOG_FATAL;
    else if (av_level <= AV_LOG_FATAL)      ijk_level = IJK_LOG_FATAL;
    else if (av_level <= AV_LOG_ERROR)      ijk_level = IJK_LOG_ERROR;
    else if (av_level <= AV_LOG_WARNING)    ijk_level = IJK_LOG_WARN;
    else if (av_level <= AV_LOG_INFO)       ijk_level = IJK_LOG_INFO;
        // AV_LOG_VERBOSE means detailed info
    else if (av_level <= AV_LOG_VERBOSE)    ijk_level = IJK_LOG_INFO;
    else if (av_level <= AV_LOG_DEBUG)      ijk_level = IJK_LOG_DEBUG;
    else if (av_level <= AV_LOG_TRACE)      ijk_level = IJK_LOG_VERBOSE;
    else                                    ijk_level = IJK_LOG_VERBOSE;
    return ijk_level;
}

static void ffp_log_callback_report(void *ptr, int level, const char *fmt, va_list vl)
{
    if (level > av_log_get_level())
        return;

    int ffplv __unused = log_level_av_to_ijk(level);

    va_list vl2;
    char line[1024];
    static int print_prefix = 1;

    va_copy(vl2, vl);
    // av_log_default_callback(ptr, level, fmt, vl);
    av_log_format_line(ptr, level, fmt, vl2, line, sizeof(line), &print_prefix);
    va_end(vl2);

    LOGD("IJK_LOG_TAG %s", line);
}


inline static int log_level_ijk_to_av(int ijk_level)
{
    int av_level = IJK_LOG_VERBOSE;
    if      (ijk_level >= IJK_LOG_SILENT)   av_level = AV_LOG_QUIET;
    else if (ijk_level >= IJK_LOG_FATAL)    av_level = AV_LOG_FATAL;
    else if (ijk_level >= IJK_LOG_ERROR)    av_level = AV_LOG_ERROR;
    else if (ijk_level >= IJK_LOG_WARN)     av_level = AV_LOG_WARNING;
    else if (ijk_level >= IJK_LOG_INFO)     av_level = AV_LOG_INFO;
        // AV_LOG_VERBOSE means detailed info
    else if (ijk_level >= IJK_LOG_DEBUG)    av_level = AV_LOG_DEBUG;
    else if (ijk_level >= IJK_LOG_VERBOSE)  av_level = AV_LOG_TRACE;
    else if (ijk_level >= IJK_LOG_DEFAULT)  av_level = AV_LOG_TRACE;
    else if (ijk_level >= IJK_LOG_UNKNOWN)  av_level = AV_LOG_TRACE;
    else                                    av_level = AV_LOG_TRACE;
    return av_level;
}

static void ffp_log_callback_brief(void *ptr, int level, const char *fmt, va_list vl)
{
    if (level > av_log_get_level())
        return;

    int ffplv __unused = log_level_av_to_ijk(level);
    LOGD(fmt, vl);
}

void FFPlayer::gSetFFLogReport(int use_report) {
    if (use_report) {
        av_log_set_callback(ffp_log_callback_report);
    } else {
        av_log_set_callback(ffp_log_callback_brief);
    }
}

void FFPlayer::gSetFFLogLevel(int log_level) {
        int av_level = log_level_ijk_to_av(log_level);
        av_log_set_level(av_level);
}

void FFPlayer::gSetInjectCallback(ff_inject_callback cb) {
    gFFInjectCallback = cb;
}

void FFPlayer::resetInternal() {
    /* is closed in stream_close() */

    /* format/codec options */
    av_dict_free(&format_opts);
    av_dict_free(&codec_opts);
    av_dict_free(&sws_dict);
    av_dict_free(&player_opts);
    av_dict_free(&swr_opts);
    av_dict_free(&swr_preset_opts);

    /* ffplay options specified by the user */
    av_freep(&input_filename);
    audio_disable          = 0;
    video_disable          = 0;
    memset(wanted_stream_spec, 0, sizeof(wanted_stream_spec));
    seek_by_bytes          = -1;
    display_disable        = 0;
    show_status            = 0;
    av_sync_type           = AV_SYNC_AUDIO_MASTER;
    start_time             = AV_NOPTS_VALUE;
    duration               = AV_NOPTS_VALUE;
    fast                   = 1;
    genpts                 = 0;
    lowres                 = 0;
    decoder_reorder_pts    = -1;
    autoexit               = 0;
    loop                   = 1;
    framedrop              = 0; // option
    seek_at_start          = 0;
    infinite_buffer        = -1;
    show_mode              = MediaState::SHOW_MODE_NONE;
    av_freep(&audio_codec_name);
    av_freep(&video_codec_name);
    rdftspeed              = 0.02;
#if CONFIG_AVFILTER
    av_freep(&vfilters_list);
    nb_vfilters            = 0;
    afilters               = NULL;
    vfilter0               = NULL;
#endif
    autorotate             = 1;

    sws_flags              = SWS_FAST_BILINEAR;

    /* current context */
    audio_callback_time    = 0;

    /* extra fields */
    aout                   = NULL; /* reset outside */
    vout                   = NULL; /* reset outside */
    pipeline               = NULL;
    node_vdec              = NULL;
    sar_num                = 0;
    sar_den                = 0;

    av_freep(&video_codec_info);
    av_freep(&audio_codec_info);
    av_freep(&subtitle_codec_info);
    overlay_format         = SDL_FCC_RV32;

    last_error             = 0;
    prepared               = 0;
    auto_resume            = 0;
    error                  = 0;
    error_count            = 0;
    start_on_prepared      = 1;
    first_video_frame_rendered = 0;
    sync_av_start          = 1;

    playable_duration_ms           = 0;

    packet_buffering               = 1;
    pictq_size                     = VIDEO_PICTURE_QUEUE_SIZE_DEFAULT; // option
    max_fps                        = 31; // option

    videotoolbox                   = 0; // option
    vtb_max_frame_width            = 0; // option
    vtb_async                      = 0; // option
    vtb_handle_resolution_change   = 0; // option
    vtb_wait_async                 = 0; // option

    mediacodec_all_videos          = 0; // option
    mediacodec_avc                 = 0; // option
    mediacodec_hevc                = 0; // option
    mediacodec_mpeg2               = 0; // option
    mediacodec_handle_resolution_change = 0; // option
    mediacodec_auto_rotate         = 0; // option

    opensles                       = 0; // option

    iformat_name                   = NULL; // option

    no_time_adjust                 = 0; // option

    ijkmeta_reset(meta);

    SDL_SpeedSamplerReset(&vfps_sampler);
    SDL_SpeedSamplerReset(&vdps_sampler);

    /* filters */
    vf_changed                     = 0;
    af_changed                     = 0;
    pf_playback_rate               = 1.0f;
    pf_playback_rate_changed       = 0;
    pf_playback_volume             = 1.0f;
    pf_playback_volume_changed     = 0;

//    av_application_closep(&app_ctx);

    msg_queue->flushMsgQueue();

    inject_opaque = NULL;
    stat->resetStatistic();
    dcc.resetDemuxCacheControl();
}







