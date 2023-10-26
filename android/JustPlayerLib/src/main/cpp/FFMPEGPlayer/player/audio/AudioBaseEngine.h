//
// Created by Peter Xi on 2022/5/29.
//

#ifndef ANDROIDTEST_AUDIOBASEENGINE_H
#define ANDROIDTEST_AUDIOBASEENGINE_H

#include <XThread.h>
#include "../../../base/util/Util.h"
#include "../../../base/config/constants.h"
#include "AudioConst.h"
#include "../FFMPEGPlayer.h"
#include <sstream>
#include "../../util/JniHelper.h"
#include "../../util/Lock.h"

class FFMPEGPlayer;

#define SDL_AUDIO_MASK_BITSIZE       (0xFF)
#define SDL_AUDIO_MASK_DATATYPE      (1<<8)
#define SDL_AUDIO_MASK_ENDIAN        (1<<12)
#define SDL_AUDIO_MASK_SIGNED        (1<<15)
#define SDL_AUDIO_BITSIZE(x)         (x & SDL_AUDIO_MASK_BITSIZE)
#define SDL_AUDIO_ISFLOAT(x)         (x & SDL_AUDIO_MASK_DATATYPE)
#define SDL_AUDIO_ISBIGENDIAN(x)     (x & SDL_AUDIO_MASK_ENDIAN)
#define SDL_AUDIO_ISSIGNED(x)        (x & SDL_AUDIO_MASK_SIGNED)
#define SDL_AUDIO_ISINT(x)           (!SDL_AUDIO_ISFLOAT(x))
#define SDL_AUDIO_ISLITTLEENDIAN(x)  (!SDL_AUDIO_ISBIGENDIAN(x))
#define SDL_AUDIO_ISUNSIGNED(x)      (!SDL_AUDIO_ISSIGNED(x))

typedef void (*AudioCallback) (void *userdata, uint8_t * stream,
                                   int len);

typedef struct JustAudioSpec
{
    int freq;                   /**< DSP frequency -- samples per second */
    uint16_t format;         /**< Audio data format */
    uint8_t channels;             /**< Number of channels: 1 mono, 2 stereo */
    uint8_t silence;              /**< Audio buffer silence value (calculated) */
    uint16_t samples;             /**< Audio buffer size in samples (power of 2) */
    uint16_t padding;             /**< NOT USED. Necessary for some compile environments */
    uint32_t size;                /**< Audio buffer size in bytes (calculated) */
    AudioCallback callback;
    void *userdata;
};

class AudioBaseEngine {

public:
    virtual int open(JustAudioSpec *desired,JustAudioSpec *obtained) =0;
    virtual void pause(bool pause)=0;
    virtual void flush()=0;
    virtual void setVolume(float left,float right) = 0 ;
    virtual void close() = 0;


    virtual double getLatencySeconds() = 0;
    virtual void setDefLatencySeconds(double latency) = 0;

    //option
    virtual void setPlaybackRate(float rate) = 0;
    virtual void setPlayVolume(float vol) = 0;
    virtual int getAudioPresecondCallBacks() = 0;

    //in android
    virtual int getAudioSessionId() = 0;

public:
    volatile bool need_flush;
    volatile bool pause_on;
    volatile bool abort_request;

    volatile bool need_set_volume;
    volatile float left_volume;
    volatile float right_volume;

    JustAudioSpec *audio_spec;
    FFMPEGPlayer* player;
};









#endif //ANDROIDTEST_AUDIOBASEENGINE_H
