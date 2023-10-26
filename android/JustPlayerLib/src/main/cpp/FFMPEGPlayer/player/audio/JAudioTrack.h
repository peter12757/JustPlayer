//
// Created by Peter Xi on 2022/5/31.
//

#ifndef ANDROIDTEST_JAUDIOTRACK_H
#define ANDROIDTEST_JAUDIOTRACK_H

#include "../../../util/JniHelper.h"

/**
 * Java中audiotrack相关的方法等都在这里，方便调用
 */
class JAudioTrack {

public:
    jobject jAudioTrack;
    jclass jAudioTrackClass;

    jmethodID constructor_AudioTrack;
    jmethodID method_getMinBufferSize;
    jmethodID method_getMaxVolume;
    jmethodID method_getMinVolume;
    jmethodID method_getNativeOutputSampleRate;
    jmethodID method_play;
    jmethodID method_pause;
    jmethodID method_stop;
    jmethodID method_flush;
    jmethodID method_release;
    jmethodID method_write;
    jmethodID method_setStereoVolume;
    jmethodID method_getAudioSessionId;
    jmethodID method_getPlaybackParams;
    jmethodID method_setPlaybackParams;
    jmethodID method_getStreamType;
    jmethodID method_getSampleRate;
    jmethodID method_getPlaybackRate;
    jmethodID method_setPlaybackRate;

    JAudioTrack(){

        jAudioTrackClass = JniHelper::FindClassAsGlobalRef("android/media/AudioTrack");

        constructor_AudioTrack = JniHelper::GetMethodID(jAudioTrackClass, "<init>", "(IIIIII)V");

        method_getMinBufferSize = JniHelper::GetStaticMethodID(jAudioTrackClass, "getMinBufferSize", "(III)I");

        method_getMaxVolume = JniHelper::GetStaticMethodID(jAudioTrackClass, "getMaxVolume", "()F");

        method_getMinVolume = JniHelper::GetStaticMethodID(jAudioTrackClass, "getMinVolume", "()F");

        method_getNativeOutputSampleRate = JniHelper::GetStaticMethodID(jAudioTrackClass, "getNativeOutputSampleRate", "(I)I");

        method_play = JniHelper::GetMethodID(jAudioTrackClass, "play", "()V");

        method_pause = JniHelper::GetMethodID(jAudioTrackClass, "pause", "()V");

        method_stop = JniHelper::GetMethodID(jAudioTrackClass, "stop", "()V");
        method_flush = JniHelper::GetMethodID(jAudioTrackClass, "flush", "()V");

        method_release = JniHelper::GetMethodID(jAudioTrackClass, "release", "()V");

        method_write = JniHelper::GetMethodID(jAudioTrackClass, "write", "([BII)I");
        method_setStereoVolume = JniHelper::GetMethodID(jAudioTrackClass, "setStereoVolume", "(FF)I");

        method_getAudioSessionId = JniHelper::GetMethodID(jAudioTrackClass, "getAudioSessionId", "()I");

        if (JniHelper::GetSystemAndroidApiLevel() >= 23) {
            method_getPlaybackParams = JniHelper::GetMethodID(jAudioTrackClass, "getPlaybackParams", "()Landroid/media/PlaybackParams;");

            method_setPlaybackParams = JniHelper::GetMethodID(jAudioTrackClass, "setPlaybackParams", "(Landroid/media/PlaybackParams;)V");
        }

        method_getStreamType = JniHelper::GetMethodID(jAudioTrackClass, "getStreamType", "()I");

        method_getSampleRate = JniHelper::GetMethodID(jAudioTrackClass, "getSampleRate", "()I");

        method_getPlaybackRate = JniHelper::GetMethodID(jAudioTrackClass, "getPlaybackRate", "()I");

        method_setPlaybackRate = JniHelper::GetMethodID(jAudioTrackClass, "setPlaybackRate", "(I)I");
    }

    ~JAudioTrack(){
        if (jAudioTrack) {
            JniHelper::deleteGlobalRef(jAudioTrack);
            jAudioTrack = NULL;
        }
    }
};

#endif //ANDROIDTEST_JAUDIOTRACK_H
