//
// Created by Peter Xi on 2022/5/29.
//

#include "AndroidAudioTrackEngine.h"
#include <math.h>

AndroidAudioTrackEngine::AndroidAudioTrackEngine()
:XThread("AndroidAudioTrackEngine", 100) {
    pHandle = new AudioEventHandle();
}

AndroidAudioTrackEngine::~AndroidAudioTrackEngine() {
    SafeDelete(pHandle);

}

int AndroidAudioTrackEngine::open(JustAudioSpec *desired, JustAudioSpec *obtained) {
    assert(desired);
    std::ostringstream logOs;
    logOs<<"AndroidAudioTrackEngine::open";
    audio_spec = desired;
    audio_track = new JustAndroid_AudioTrack();
    audio_track->spec = createAudioTrackSpec(desired);
    audio_track->jAudioTrack = new JAudioTrack();
    buffer_size = audio_track->min_buffer_size;
    buffer = static_cast<char *>(malloc(buffer_size));
    if (!buffer) {
        logOs<<"buffer malloc fail!!!";
    }
    if (obtained) {
        audio_spec->freq = audio_track->spec->sample_rate_in_hz;
        audio_spec->channels = findAndroidChannel(audio_track->spec->channel_config);
        audio_spec->format = findAndroidFormat(audio_track->spec->audio_format);
        audio_spec->size = audio_track->spec->buffer_size_in_bytes;
        audio_spec->silence = 0;
        audio_spec->padding = 0;
    }
    JNIEnv *env;
    ATTACH_JVM(env)
    audio_session_id = env->CallIntMethod(audio_track->jAudioTrack->jAudioTrack,audio_track->jAudioTrack->method_getAudioSessionId);
    logOs<<"audio_session_id: "<<audio_session_id;

    pause_on = 1;
    abort_request = 0;
    DETACH_JVM(env)
    startThread();
    LOGD("%s",logOs.str().c_str());
    return 0;

}

void AndroidAudioTrackEngine::pause(bool pause) {
    pause_on = pause;

}

void AndroidAudioTrackEngine::flush() {
    need_flush = true;
}

void AndroidAudioTrackEngine::setVolume(float left, float right) {
    need_set_volume =true;
    left_volume = left;
    right_volume = right;

}

void AndroidAudioTrackEngine::close() {
    abort_request = true;

}

double AndroidAudioTrackEngine::getLatencySeconds() {
    return 0;
}

void AndroidAudioTrackEngine::setDefLatencySeconds(double latency) {


}

void AndroidAudioTrackEngine::setPlaybackRate(float rate) {

}

void AndroidAudioTrackEngine::setPlayVolume(float vol) {

}

int AndroidAudioTrackEngine::getAudioPresecondCallBacks() {
    return 0;
}

int AndroidAudioTrackEngine::getAudioSessionId() {
    return 0;
}

ChannelConfig AndroidAudioTrackEngine::findAndroidChannel(uint8_t channels) {
    for (int i = 0; i < NELEM(g_audio_channel_map); ++i) {
        AudioChannelMapEntry *entry = &g_audio_channel_map[i];
        if (entry->channel == channels)
            return entry->android_channel;
    }
    return CHANNEL_OUT_INVALID;
}

AudioFormat AndroidAudioTrackEngine::findAndroidFormat(uint16_t format) {
    for (int i = 0; i < NELEM(g_audio_format_map); ++i) {
        AudioFormatMapEntry *entry = &g_audio_format_map[i];
        if (entry->format == format)
            return entry->android_format;
    }
    return ENCODING_INVALID;
}

JustAndroid_AudioTrack_Spec* AndroidAudioTrackEngine::createAudioTrackSpec(JustAudioSpec *pSpec) {
    ostringstream logOs;
    JustAndroid_AudioTrack_Spec *audio_track_spec = new JustAndroid_AudioTrack_Spec();
    audio_track_spec->stream_type = STREAM_MUSIC;
    audio_track_spec->sample_rate_in_hz = 0;
    audio_track_spec->channel_config = CHANNEL_OUT_STEREO;
    audio_track_spec->audio_format = ENCODING_PCM_16BIT;
    audio_track_spec->buffer_size_in_bytes = 0;
    audio_track_spec->mode = MODE_STREAM;
    audio_track_spec->sample_rate_in_hz = audio_spec->freq;
    audio_track_spec->channel_config = findAndroidChannel(audio_spec->channels);
    audio_track_spec->audio_format = findAndroidFormat(audio_spec->format);
    audio_track_spec->buffer_size_in_bytes = audio_spec->size;

    logOs<<" SDL_Android_AudioTrack: ";

    switch (audio_track_spec->channel_config) {
        case CHANNEL_OUT_MONO:
            logOs<<"CHANNEL_OUT_MONO";
            break;
        case CHANNEL_OUT_STEREO:
            logOs<<"CHANNEL_OUT_STEREO";
            break;
        default:
            logOs<<"invalid channel "<<audio_track_spec->channel_config;
            LOGE("%s",logOs.str().c_str());
            return nullptr;
    }

    logOs<<" SDL_Android_AudioTrack: ";
    switch (audio_track_spec->audio_format) {
        case ENCODING_PCM_16BIT:
            logOs<<"ENCODING_PCM_16BIT";
            break;
        case ENCODING_PCM_8BIT:
            logOs<<"ENCODING_PCM_8BIT";
            break;
        case ENCODING_PCM_FLOAT:
            logOs<<"ENCODING_PCM_FLOAT";
            break;
        default:
            logOs<<"invalid format "<<audio_track_spec->audio_format;
            LOGE("%s",logOs.str().c_str());
            return nullptr;
    }

    // libswresample is ugly, depending on native resampler
    while (audio_track->spec->sample_rate_in_hz < 4000) {
        audio_track->spec->sample_rate_in_hz *= 2;
    }
    while (audio_track->spec->sample_rate_in_hz > 48000) {
        audio_track->spec->sample_rate_in_hz /= 2;
    }

    //getMinBufSize by callback
    GetMinBufSizePack pack(audio_track->spec->sample_rate_in_hz,
                           audio_track->spec->channel_config,audio_track->spec->audio_format);
    string buf;
    PacketToString(pack,buf);
    jbyteArray ret = static_cast<jbyteArray>(player->callJavaMethod(buf));
    JNIEnv *env;
    ATTACH_JVM(env)
    string result = JniHelper::jbyteArray2str(env,ret);
    JniHelper::deleteGlobalRef(ret);


    JustData::Package::Unpack up(result.data(), result.size());
    uint32_t uri = up.pop_uint32();
    int min_buffer_size = up.pop_uint32();
    if (min_buffer_size <= 0) {
        logOs<<"getMinBufferSize: return "<<min_buffer_size;
        free(audio_track);
        DETACH_JVM(env)
        LOGD("%s",logOs.str().c_str());
        return nullptr;
    }

    // for fast playback
    min_buffer_size *= AUDIOTRACK_PLAYBACK_MAXSPEED;

    string catchAllBuf;
    PacketToString(pack,catchAllBuf);
    jobject catchAllRet = player->callJavaMethod(catchAllBuf);

    audio_track->jAudioTrack->jAudioTrack = catchAllRet;
    if (!audio_track->jAudioTrack->jAudioTrack) {
        SafeDelete(audio_track);
        DETACH_JVM(env)
        LOGD("%s",logOs.str().c_str());
        return NULL;
    }

    audio_track->min_buffer_size = min_buffer_size;
    audio_track->spec->buffer_size_in_bytes = min_buffer_size;
    // audio_track->max_volume = J4AC_AudioTrack__getMaxVolume__catchAll(env);
    // audio_track->min_volume = J4AC_AudioTrack__getMinVolume__catchAll(env);
    audio_track->max_volume = 1.0f;
    audio_track->min_volume = 0.0f;

    // extra init
    float init_volume = 1.0f;
    init_volume = MIN(init_volume, audio_track->max_volume);
    init_volume = MAX(init_volume, audio_track->min_volume);
    logOs<<" init volume as "<<init_volume<<"/("<<audio_track->min_volume<<","<<audio_track->max_volume<<")";
    env->CallIntMethod(audio_track->jAudioTrack->jAudioTrack,audio_track->jAudioTrack->method_setStereoVolume,
                                  init_volume,init_volume);
    DETACH_JVM(env)
    LOGD("%s",logOs.str().c_str());
    return audio_track_spec;
}


void AndroidAudioTrackEngine::onThreadRun(uint32_t now) {
    std::ostringstream logOs;
    logOs<<"AndroidAudioTrackEngine::onThreadRun";
    LOGD("%s",logOs.str().c_str());
//    pHandle->onEventRun();
}

void AndroidAudioTrackEngine::postCall(int *event) {
//    if (pHandle)
//        pHandle->onEventRun();
}





