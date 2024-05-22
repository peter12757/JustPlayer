//
// Created by Peter Xi on 2022/6/2.
//
#ifndef ANDROIDTEST_AUDIOEVENTHANDLE_H
#define ANDROIDTEST_AUDIOEVENTHANDLE_H


#include "../../../util/thread/IEventHandler.h"
#include <sstream>


class IEventHandler;

enum AUDIO_EVENT {
    EVENT_AUDIO_UNKNOW,
    EVENT_AUDIO_PAUSE,
    EVENT_AUDIO_FLUSH,
    EVENT_AUDIO_SET_VOLUME,
    EVENT_AUDIO_CLOSE,
    EVENT_AUDIO_SET_PLAYBACK,
    EVENT_AUDIO_SET_PLAYVOLUME,
};

class IAudioEvent: public IBaseEvent
{
public:
    IAudioEvent()
            : IBaseEvent(EVENT_AUDIO_UNKNOW)
    {
    }
    virtual ~IAudioEvent() {}

    IAudioEvent* createDup() { return this; }
    virtual void deleteDup() {}

};

class AudioEventHandle : public IEventHandler{
public:
    AudioEventHandle();
    virtual ~AudioEventHandle();

};


#endif //ANDROIDTEST_AUDIOEVENTHANDLE_H
