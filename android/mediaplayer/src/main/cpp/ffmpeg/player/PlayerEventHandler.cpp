//
// Created by Peter Xi on 2022/6/18.
//
#include <sstream>
#include "PlayerEventHandler.h"

PlayerEventHandler::PlayerEventHandler() {
    std::ostringstream  logOs;
    logOs<<"PlayerEventHandler::PlayerEventHandler";
    LOGD("%s",logOs.str().c_str());
}

PlayerEventHandler::~PlayerEventHandler() {
    std::ostringstream logOs;
    logOs<<"PlayerEventHandler::~PlayerEventHandler";
    LOGD("%s",logOs.str().c_str());
}

void PlayerEventHandler::initHandler() {
    
}
