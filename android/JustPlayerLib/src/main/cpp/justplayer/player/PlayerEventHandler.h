//
// Created by Peter Xi on 2022/6/18.
//

#pragma once

#ifndef ANDROIDTEST_PLAYEREVENTHANDLER_H
#define ANDROIDTEST_PLAYEREVENTHANDLER_H

#include "../../util/thread/IEventHandler.h"

enum PLAYER_EVENT {
    EVENT_PLAYER_UNKNOW,
    EVENT_PLAYER_PAUSE,
    EVENT_PLAYER_FLUSH,
    EVENT_PLAYER_SET_VOLUME,
    EVENT_PLAYER_CLOSE,
    EVENT_PLAYER_SET_PLAYBACK,
    EVENT_PLAYER_SET_PLAYVOLUME,
};

class PlayerEvent: public IBaseEvent {

public:
    PlayerEvent(): IBaseEvent(EVENT_PLAYER_UNKNOW){}
    virtual ~PlayerEvent(){};

    PlayerEvent* createDup() {return this;};
    virtual void deleteDup(){}
};

class PlayerEventHandler : public IEventHandler {

public:
    PlayerEventHandler();
    virtual ~PlayerEventHandler();

    virtual void initHandler();
};


#endif //ANDROIDTEST_PLAYEREVENTHANDLER_H
