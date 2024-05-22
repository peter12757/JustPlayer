//
// Created by Peter Xi on 2022/5/29.
//

#ifndef ANDROIDTEST_ANDROIDAUDIOTRACKENGINE_H
#define ANDROIDTEST_ANDROIDAUDIOTRACKENGINE_H

#include "AudioBaseEngine.h"
#include "JAudioTrack.h"
#include "AudioEventHandle.h"


#define AUDIOTRACK_PLAYBACK_MAXSPEED (2)



struct JustAndroid_AudioTrack_Spec {
    StreamType stream_type;
    ChannelConfig channel_config;
    AudioFormat audio_format;
    Mode mode;
    WriteMode write_mode;   // not used

    int sample_rate_in_hz;
    int buffer_size_in_bytes;
    // extra field
    int sdl_samples;
};

struct JustAndroid_AudioTrack {
    JAudioTrack* jAudioTrack;
    JustAndroid_AudioTrack_Spec* spec;
    char * byte_buffer;
    int         byte_buffer_capacity;
    int         min_buffer_size;
    float       max_volume;
    float       min_volume;
} ;

enum AudioMethod {
    Method_GetMinBufferSize=1,
    Method_CatchAll = Method_GetMinBufferSize+1,

};

struct GetMinBufSizePack :Marshallable {
    GetMinBufSizePack(int samplaRateInHz, ChannelConfig channelConfig, AudioFormat audioFormat)
    :sampleRateInHz(samplaRateInHz)
    ,channelConfig(channelConfig)
    ,audioFormat(audioFormat){}

    uint32_t sampleRateInHz;
    uint32_t channelConfig;
    uint32_t audioFormat;

    uint32_t size() const override {
        return sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t);
    }

    void marshal(Pack &p) const override{p.push_uint32(AudioMethod::Method_GetMinBufferSize)<<sampleRateInHz<<channelConfig<<audioFormat;}

};

struct CatchAllPack :Marshallable {
    CatchAllPack(uint32_t samplaRateInHz, ChannelConfig channelConfig,
                 AudioFormat audioFormat, StreamType streamType, uint32_t minBufferSize, ThreadMode mode)
            :sampleRateInHz(samplaRateInHz)
            ,channelConfig(channelConfig)
            ,audioFormat(audioFormat)
            ,streamType(streamType)
            ,minBufferSize(minBufferSize)
            ,mode(mode){}

    uint32_t streamType;
    uint32_t sampleRateInHz;
    uint32_t channelConfig;
    uint32_t audioFormat;
    uint32_t minBufferSize;
    uint32_t mode;

    uint32_t size() const override {
        return sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t)+sizeof(uint32_t);
    }

    void marshal(Pack &p) const override{p.push_uint32(AudioMethod::Method_CatchAll)<<streamType
    <<sampleRateInHz<<channelConfig<<audioFormat<< minBufferSize<<mode;}

};



class AndroidAudioTrackEngine : public AudioBaseEngine ,XThread{
public:
    AndroidAudioTrackEngine();
    virtual ~AndroidAudioTrackEngine();

    virtual int open(JustAudioSpec *desired,JustAudioSpec *obtained);
    virtual void pause(bool pause);
    virtual void flush();
    virtual void setVolume(float left,float right) ;
    virtual void close();

    virtual double getLatencySeconds();
    virtual void setDefLatencySeconds(double latency);


    //option
    virtual void setPlaybackRate(float rate);
    virtual void setPlayVolume(float vol);
    virtual int getAudioPresecondCallBacks();

    //in android
    virtual int getAudioSessionId();

    ChannelConfig findAndroidChannel(uint8_t channels);

    AudioFormat findAndroidFormat(uint16_t format);

    JustAndroid_AudioTrack_Spec* createAudioTrackSpec(JustAudioSpec *pSpec);

    //thread
    virtual void onThreadRun(uint32_t now);
    virtual void postCall(int *event);

protected:
    //data
    JustAndroid_AudioTrack *audio_track;

    volatile bool need_flush;
    volatile bool pause_on;
    volatile bool abort_request;

    volatile bool need_set_volume;
    volatile float left_volume;
    volatile float right_volume;
    char *buffer;
    uint32_t buffer_size;

    uint32_t audio_session_id;

//thread
    AudioEventHandle* pHandle;
};


#endif //ANDROIDTEST_ANDROIDAUDIOTRACKENGINE_H
