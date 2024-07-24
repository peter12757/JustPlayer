//
// Created by Administrator on 2024/7/24.
//

#ifndef JUSTPLAYER_MEDIAMETA_H
#define JUSTPLAYER_MEDIAMETA_H


#include "FFinc.h"

// media meta
#define IJKM_KEY_FORMAT         "format"
#define IJKM_KEY_DURATION_US    "duration_us"
#define IJKM_KEY_START_US       "start_us"
#define IJKM_KEY_BITRATE        "bitrate"
#define IJKM_KEY_VIDEO_STREAM   "video"
#define IJKM_KEY_AUDIO_STREAM   "audio"
#define IJKM_KEY_TIMEDTEXT_STREAM "timedtext"

// stream meta
#define IJKM_KEY_TYPE           "type"
#define IJKM_VAL_TYPE__VIDEO    "video"
#define IJKM_VAL_TYPE__AUDIO    "audio"
#define IJKM_VAL_TYPE__TIMEDTEXT "timedtext"
#define IJKM_VAL_TYPE__UNKNOWN  "unknown"
#define IJKM_KEY_LANGUAGE       "language"

#define IJKM_KEY_CODEC_NAME         "codec_name"
#define IJKM_KEY_CODEC_PROFILE      "codec_profile"
#define IJKM_KEY_CODEC_LEVEL        "codec_level"
#define IJKM_KEY_CODEC_LONG_NAME    "codec_long_name"
#define IJKM_KEY_CODEC_PIXEL_FORMAT "codec_pixel_format"
#define IJKM_KEY_CODEC_PROFILE_ID   "codec_profile_id"

// stream: video
#define IJKM_KEY_WIDTH          "width"
#define IJKM_KEY_HEIGHT         "height"
#define IJKM_KEY_FPS_NUM        "fps_num"
#define IJKM_KEY_FPS_DEN        "fps_den"
#define IJKM_KEY_TBR_NUM        "tbr_num"
#define IJKM_KEY_TBR_DEN        "tbr_den"
#define IJKM_KEY_SAR_NUM        "sar_num"
#define IJKM_KEY_SAR_DEN        "sar_den"
// stream: audio
#define IJKM_KEY_SAMPLE_RATE    "sample_rate"
#define IJKM_KEY_CHANNEL_LAYOUT "channel_layout"

// reserved for user
#define IJKM_KEY_STREAMS        "streams"


#define IJK_META_INIT_CAPACITY 13

class MediaMeta {

public:
//    SDL_mutex *mutex;

    AVDictionary *dict;

    size_t children_count;
    size_t children_capacity;
    MediaMeta **children;

    MediaMeta();
    ~MediaMeta();

    void reset();

    void appendChild(MediaMeta *child);
    void set_int64_l(const char *name, int64_t value);
    void set_string_l(const char *name, const char *value);
    void set_avformat_context_l(struct AVFormatContext *ic);

// must be freed with free();
    const char   *get_string_l(const char *name);
    int64_t       get_int64_l(const char *name, int64_t defaultValue);
    size_t        get_children_count_l();
// do not free
    MediaMeta *get_child_l(size_t index);

};




#endif //JUSTPLAYER_MEDIAMETA_H
