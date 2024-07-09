//
// Created by Peter Xi on 2022/6/16.
//

#ifndef ANDROIDTEST_PLAYERHANDLER_H
#define ANDROIDTEST_PLAYERHANDLER_H

#include "../../../base/thread/IEventHandler.h"

enum PlayerEventType {
    EVENT_UNKNOW,
    EVENT_PAUSE,
    EVENT_FLUSH,
    SET_VOLUME,
    CLOSE,
    SET_PLAYBACK,
    SET_PLAYVOLUME,
};

class IPlayerEvent: public IBaseEvent
{
public:
    IPlayerEvent()
            : IBaseEvent(EVENT_UNKNOW)
    {
    }
    virtual ~IPlayerEvent() {}

    IPlayerEvent* createDup() { return this; }
    virtual void deleteDup() {}

};


class PlayerHandler : public IEventHandler{

};


#endif //ANDROIDTEST_PLAYERHANDLER_H
