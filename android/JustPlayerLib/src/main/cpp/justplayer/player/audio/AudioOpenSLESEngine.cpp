//
// Created by Peter Xi on 2022/5/29.
//

#include "AudioOpenSLESEngine.h"


AudioOpenSLESEngine::AudioOpenSLESEngine()
: XThread("AudioOpenSLESEngine",100)
{

}

AudioOpenSLESEngine::~AudioOpenSLESEngine() {

}

int AudioOpenSLESEngine::open(JustAudioSpec *desired, JustAudioSpec *obtained) {
    return 0;
}

void AudioOpenSLESEngine::pause(bool pause) {

}

void AudioOpenSLESEngine::flush() {

}

void AudioOpenSLESEngine::setVolume(float left, float right) {

}

void AudioOpenSLESEngine::close() {
    NOT_IMPL;
}

double AudioOpenSLESEngine::getLatencySeconds() {
    return 0;
}

void AudioOpenSLESEngine::setDefLatencySeconds(double latency) {


}

void AudioOpenSLESEngine::setPlaybackRate(float rate) {


}

void AudioOpenSLESEngine::setPlayVolume(float vol) {

}

int AudioOpenSLESEngine::getAudioPresecondCallBacks() {
    return 0;
}

int AudioOpenSLESEngine::getAudioSessionId() {
    return 0;
}

void AudioOpenSLESEngine::onThreadRun(uint32_t now) {
    std::ostringstream logOs;
    logOs<<"AudioOpenSLESEngine::onThreadRun ";
    LOGD("%s",logOs.str().c_str());
}

void AudioOpenSLESEngine::postCall(IAudioEvent *event) {
    return;
}