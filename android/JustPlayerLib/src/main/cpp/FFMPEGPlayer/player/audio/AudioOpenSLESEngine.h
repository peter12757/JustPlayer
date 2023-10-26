//
// Created by Peter Xi on 2022/5/29.
//

#ifndef ANDROIDTEST_AUDIOOPENSLESENGINE_H
#define ANDROIDTEST_AUDIOOPENSLESENGINE_H

#include "AudioEventHandle.h"
#include "AudioBaseEngine.h"


class AudioOpenSLESEngine :public AudioBaseEngine,XThread {
public:
    AudioOpenSLESEngine();
    virtual ~AudioOpenSLESEngine();

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

    void postCall(IAudioEvent *event);

    //thread
    void onThreadRun(uint32_t now) override;
};


#endif //ANDROIDTEST_AUDIOOPENSLESENGINE_H
