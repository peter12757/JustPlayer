//
// Created by Peter Xi on 2022/6/3.
//

#ifndef ANDROIDTEST_AUDIOOBJ_H
#define ANDROIDTEST_AUDIOOBJ_H
#include "FFObj.h"
#include <sstream>
#include "jni.h"


#define SAMPLE_QUEUE_SIZE 9

/* we use about AUDIO_DIFF_AVG_NB A-V differences to make the average */
#define AUDIO_DIFF_AVG_NB   20

typedef uint16_t AudioFormat;

class AudioSpec {
public:
    int freq;                   /**< DSP frequency -- samples per second */
    AudioFormat format;     /**< Audio data format */
    uint8_t channels;             /**< Number of channels: 1 mono, 2 stereo */
    uint8_t silence;              /**< Audio buffer silence value (calculated) */
    uint16_t samples;             /**< Audio buffer size in samples (power of 2) */
    uint16_t padding;             /**< NOT USED. Necessary for some compile environments */
    uint32_t size;                /**< Audio buffer size in bytes (calculated) */
    void *userdata;

    //SDL_AudioCallback callback;
    void (*Audio_Callback) (void *userdata, uint8_t * stream,
                               int len);
} AudioSpec;

typedef struct Android_AudioTrack_Spec {
    enum StreamType {
        STREAM_VOICE_CALL = 0,
        STREAM_SYSTEM = 1,
        STREAM_RING = 2,
        STREAM_MUSIC = 3,
        STREAM_ALARM = 4,
        STREAM_NOTIFICATION = 5,
    } stream_type;

    int sample_rate_in_hz;

    enum ChannelConfig {
        CHANNEL_OUT_INVALID = 0x0,
        CHANNEL_OUT_DEFAULT = 0x1, /* f-l */
        CHANNEL_OUT_MONO = 0x4, /* f-l, f-r */
        CHANNEL_OUT_STEREO = 0xc, /* f-l, f-r, b-l, b-r */
        CHANNEL_OUT_QUAD = 0xcc, /* f-l, f-r, b-l, b-r */
        CHANNEL_OUT_SURROUND = 0x41c, /* f-l, f-r, f-c, b-c */
        CHANNEL_OUT_5POINT1 = 0xfc, /* f-l, f-r, b-l, b-r, f-c, low */
        CHANNEL_OUT_7POINT1 = 0x3fc, /* f-l, f-r, b-l, b-r, f-c, low, f-lc, f-rc */

        CHANNEL_OUT_FRONT_LEFT = 0x4,
        CHANNEL_OUT_FRONT_RIGHT = 0x8,
        CHANNEL_OUT_BACK_LEFT = 0x40,
        CHANNEL_OUT_BACK_RIGHT = 0x80,
        CHANNEL_OUT_FRONT_CENTER = 0x10,
        CHANNEL_OUT_LOW_FREQUENCY = 0x20,
        CHANNEL_OUT_FRONT_LEFT_OF_CENTER = 0x100,
        CHANNEL_OUT_FRONT_RIGHT_OF_CENTER = 0x200,
        CHANNEL_OUT_BACK_CENTER = 0x400,
    } channel_config;

    enum AudioFormat {
        ENCODING_INVALID = 0,
        ENCODING_DEFAULT = 1,
        ENCODING_PCM_16BIT = 2, // signed, guaranteed to be supported by devices.
        ENCODING_PCM_8BIT = 3, // unsigned, not guaranteed to be supported by devices.
        ENCODING_PCM_FLOAT = 4, // single-precision floating-point per sample
    } audio_format;
    int buffer_size_in_bytes;

    enum Mode {
        MODE_STATIC = 0,
        MODE_STREAM = 1,
    } mode;

    enum WriteMode {
        WRITE_BLOCKING     = 0,
        WRITE_NON_BLOCKING = 1,
    } write_mode; // not used

    // extra field
    int sdl_samples;
} Android_AudioTrack_Spec;

class Android_AudioTrack {
    jobject thiz;

    Android_AudioTrack_Spec spec;

    jbyteArray  byte_buffer;
    int         byte_buffer_capacity;
    int         min_buffer_size;
    float       max_volume;
    float       min_volume;
} Android_AudioTrack;

class Aout_Opaque {
//    SDL_cond *wakeup_cond;
//    SDL_mutex *wakeup_mutex;

    class AudioSpec spec;
    class Android_AudioTrack* atrack;
    uint8_t *buffer;
    int buffer_size;

    volatile bool need_flush;
    volatile bool pause_on;
    volatile bool abort_request;

    volatile bool need_set_volume;
    volatile float left_volume;
    volatile float right_volume;

//    SDL_Thread *audio_tid;
//    SDL_Thread _audio_tid;

    int audio_session_id;

    volatile float speed;
    volatile bool speed_changed;
} Aout_Opaque;


class Aout {
public:
    Aout();
    virtual ~Aout();


public:
//    SDL_mutex *mutex;
    double     minimal_latency_seconds;

    const char       *opaque_class;
    class Aout_Opaque *opaque;
    void (*free_l)(Aout *aout);
    int (*open_audio)(Aout *aout, const class AudioSpec *desired, class AudioSpec *obtained);
    void (*pause_audio)(Aout *aout, int pause_on);
    void (*flush_audio)(Aout *aout);
    void (*set_volume)(Aout *aout, float left, float right);
    void (*close_audio)(Aout *aout);

    double (*func_get_latency_seconds)(Aout *aout);
    void   (*func_set_default_latency_seconds)(Aout *aout, double latency);

    // optional
    void   (*func_set_playback_rate)(Aout *aout, float playbackRate);
    void   (*func_set_playback_volume)(Aout *aout, float playbackVolume);
    int    (*func_get_audio_persecond_callbacks)(Aout *aout);

    // Android only
    int    (*func_get_audio_session_id)(Aout *aout);
};





#endif //ANDROIDTEST_AUDIOOBJ_H
