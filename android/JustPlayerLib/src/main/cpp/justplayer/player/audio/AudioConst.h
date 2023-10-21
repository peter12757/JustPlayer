//
// Created by Peter Xi on 2022/5/30.
//

#ifndef ANDROIDTEST_AUDIOCONST_H
#define ANDROIDTEST_AUDIOCONST_H

enum StreamType {
    STREAM_VOICE_CALL = 0,
    STREAM_SYSTEM = 1,
    STREAM_RING = 2,
    STREAM_MUSIC = 3,
    STREAM_ALARM = 4,
    STREAM_NOTIFICATION = 5,
};

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
} ;

enum AudioFormat {
    ENCODING_INVALID = 0,
    ENCODING_DEFAULT = 1,
    ENCODING_PCM_16BIT = 2, // signed, guaranteed to be supported by devices.
    ENCODING_PCM_8BIT = 3, // unsigned, not guaranteed to be supported by devices.
    ENCODING_PCM_FLOAT = 4, // single-precision floating-point per sample
} ;

enum Mode {
    MODE_STATIC = 0,
    MODE_STREAM = 1,
} ;

enum WriteMode {
    WRITE_BLOCKING     = 0,
    WRITE_NON_BLOCKING = 1,
} ;

enum AudioEngine{
    AUDIOTRACK,
    OPENSLES,
};

typedef struct AudioChannelMapEntry {
    uint8_t channel;
    ChannelConfig android_channel;
    std::string name;
    std::string android_name;
};

static AudioChannelMapEntry g_audio_channel_map[] = {
        { 2, CHANNEL_OUT_STEREO, "2-chan", "CHANNEL_OUT_STEREO" },
        { 1, CHANNEL_OUT_MONO, "1-chan", "CHANNEL_OUT_MONO" },
};


#define AUDIO_INVALID   0x0000
#define AUDIO_U8        0x0008  /**< Unsigned 8-bit samples */
#define AUDIO_S8        0x8008  /**< Signed 8-bit samples */
#define AUDIO_U16LSB    0x0010  /**< Unsigned 16-bit samples */
#define AUDIO_S16LSB    0x8010  /**< Signed 16-bit samples */
#define AUDIO_U16MSB    0x1010  /**< As above, but big-endian byte order */
#define AUDIO_S16MSB    0x9010  /**< As above, but big-endian byte order */
#define AUDIO_U16       AUDIO_U16LSB
#define AUDIO_S16       AUDIO_S16LSB

#define AUDIO_S32LSB    0x8020  /**< 32-bit integer samples */
#define AUDIO_S32MSB    0x9020  /**< As above, but big-endian byte order */
#define AUDIO_S32       AUDIO_S32LSB

#define AUDIO_F32LSB    0x8120  /**< 32-bit floating point samples */
#define AUDIO_F32MSB    0x9120  /**< As above, but big-endian byte order */
#define AUDIO_F32       AUDIO_F32LSB


#if BYTEORDER == LIL_ENDIAN
#define AUDIO_U16SYS    AUDIO_U16LSB
#define AUDIO_S16SYS    AUDIO_S16LSB
#define AUDIO_S32SYS    AUDIO_S32LSB
#define AUDIO_F32SYS    AUDIO_F32LSB
#else
#define AUDIO_U16SYS    AUDIO_U16MSB
#define AUDIO_S16SYS    AUDIO_S16MSB
#define AUDIO_S32SYS    AUDIO_S32MSB
#define AUDIO_F32SYS    AUDIO_F32MSB
#endif
typedef struct AudioFormatMapEntry {
    uint16_t format;
    AudioFormat android_format;
    std::string name;
    std::string android_name;
} AudioFormatMapEntry;
static AudioFormatMapEntry g_audio_format_map[] = {
        { AUDIO_S16SYS, ENCODING_PCM_16BIT, "AUDIO_S16SYS", "ENCODING_PCM_16BIT" },
        { AUDIO_U8,     ENCODING_PCM_8BIT,  "AUDIO_U8",     "ENCODING_PCM_8BIT" },
        { AUDIO_F32,    ENCODING_PCM_FLOAT, "AUDIO_F32",    "ENCODING_PCM_FLOAT" },
};

#endif //ANDROIDTEST_AUDIOCONST_H
