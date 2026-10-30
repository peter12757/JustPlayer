//
// Created by Administrator on 2026/10/26.
//

#include "FFPlayer.h"
#include "../ffmpeginc/libavformat/avformat.h"
#include "common.h"




FFPlayer::FFPlayer() {
//    avformat_network_init();
    ctx = new FFContext();
    std::ostringstream logOs;
    logOs<<"create instance";
    LOGD("%s",logOs.str().c_str());
}

FFPlayer::~FFPlayer() {
    SafeDelete(ctx);
    std::ostringstream logOs;
    logOs<<"free instance";
    LOGD("%s",logOs.str().c_str());
}

int FFPlayer::getState() {
    std::ostringstream logOs;
    logOs<<"getState";
    LOGD("%s",logOs.str().c_str());
    return 0;
}

int FFPlayer::setDataSource(string source) {
    std::ostringstream logOs;
    logOs<<"setDataSource"<<source;
    LOGD("%s",logOs.str().c_str());
    return 0;
}

int FFPlayer::start() {
    std::ostringstream logOs;
    logOs<<"start";
    LOGD("%s",logOs.str().c_str());
    return 0;
}

int FFPlayer::prepare_async() {
    std::ostringstream logOs;
    logOs<<"prepare_async";
    LOGD("%s",logOs.str().c_str());
    return 0;
}

int FFPlayer::reset() {
    std::ostringstream logOs;
    logOs<<"reset";
    LOGD("%s",logOs.str().c_str());
    return 0;
}

int FFPlayer::seek(long pos) {
    std::ostringstream logOs;
    logOs<<"seek->"<<pos;
    LOGD("%s",logOs.str().c_str());
    return 0;
}

int FFPlayer::getPosition() {
    std::ostringstream logOs;
    logOs<<"getPosition->";
    LOGD("%s",logOs.str().c_str());
    return 0;
}

int FFPlayer::stop() {
    std::ostringstream logOs;
    logOs<<"stop->";
    LOGD("%s",logOs.str().c_str());
    return 0;
}

int FFPlayer::pause() {
    std::ostringstream logOs;
    logOs<<"pause->";
    LOGD("%s",logOs.str().c_str());
    return 0;
}
