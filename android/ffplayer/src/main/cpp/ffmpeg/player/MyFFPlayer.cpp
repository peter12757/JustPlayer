//
// Created by Administrator on 2024/8/4.
//


#include "MyFFPlayer.h"

MyFFPlayer::MyFFPlayer()
:av_sync_type(AV_SYNC_AUDIO_MASTER){
    videObj = new VideoObj();
    msg_queue = new MessageQueue();
}

MyFFPlayer::~MyFFPlayer() {
    SafeDelete(videObj);
}

int MyFFPlayer::setSurface(ANativeWindow *native_window) {
    std::ostringstream logOs;
    logOs<< "MyFFPlayer::setSurface";
    //todo mutex?
    if (native_window == videObj->native_window) {
        if (!native_window) {
            logOs<<"native_window is null reset";
            videObj->resetAllMediaCodecBuffers();
        }
        return 1;
    }
//    IJK_EGL_terminate(opaque->egl);   //todo egl renders
    videObj->resetAllMediaCodecBuffers();

    if (videObj->native_window)
        ANativeWindow_release(videObj->native_window);

    if (native_window)
        ANativeWindow_acquire(native_window);

    videObj->native_window = native_window;
    videObj->null_native_window_warned = 0;
    LOGD("%s",logOs.str().c_str());
    return 1;
}

int MyFFPlayer::setDataSource(std::string url) {
    std::ostringstream logOs;
    logOs<<"MyFFPlayer::setDataSource";
    if (url.empty()) logOs<< "url is null";
    if (mp_state != IDLE) logOs<<"player state err : state="<<mp_state;
    data_source = url;
    changeState(INITIALIZED);
    LOGD("%s",logOs.str().c_str());
    return 0;
}

void MyFFPlayer::changeState(PLAYER_STATE state) {
    mp_state = state;
    notifyMessage(FFP_MSG_PLAYBACK_STATE_CHANGED);
}

void MyFFPlayer::notifyMessage(int what, int arg1, int arg2, void *obj, int obj_len) {
    AVMessage *msg = new AVMessage();
    msg->what = what;
    msg->arg1 = arg1;
    msg->arg2 = arg2;
    if (obj){
        msg->obj = static_cast<void *>(av_malloc(obj_len));
        memcpy(msg->obj,obj,obj_len);
    }
    msg_queue->putMessage(msg);


}

int MyFFPlayer::prepare_async() {
    std::ostringstream logOs;
    logOs<<"MyFFPlayer::prepare_async";
    if (mp_state != INITIALIZED && mp_state != STOPPED) {
        logOs << "state err state :" << mp_state;
        return -1;
    }
    if (data_source.empty()){
        logOs<<"data_source = nullptr";
        LOGD("%s",logOs.str().c_str());
        return -1;
    }

    msg_queue->startMsgQueue();

    //prepared
    const char *source = data_source.c_str();
    if (av_stristart(source,"rtmp", nullptr) || av_stristart(source,"rtsp", nullptr)) {
        // There is total different meaning for 'timeout' option in rtmp
        logOs<<"remove 'timeout' option for rtmp.\n";
        av_dict_set(&mediaState->format_opts, "timeout", NULL, 0);
    }

    /* there is a length limit in avformat */
    if (strlen(source) + 1 > 1024) {
        logOs<<"%s too long url\n";
    }

    printVersion();
    av_opt_set_dict((void *) av_class, &mediaState->player_opts);
    if (!audioObj) {
        audioObj = new AudioObj();
//        audioObj->open();
    }

    //stream_open
    if (mediaState) {
        logOs<<"mediaState is not null";
        LOGE("%s",logOs.str().c_str());
        SafeDelete(mediaState);
    }
    mediaState = new VideoState(this,data_source, nullptr, av_sync_type);

    //video_refresh_thread started thread todo

    //read_thread started thread todo
    readthread = new ReadThread(mediaState);
    readthread->startThread();
    LOGD("%s",logOs.str().c_str());
    return 0;
}

void MyFFPlayer::printVersion() {
    std::ostringstream logOs;
    logOs<<"===== versions =====\n";
    logOs<<"FFmpeg"<<av_version_info();
    logOs<<"libavutil"<<avutil_version();
    logOs<<"libavcodec"<<avcodec_version();
    logOs<<"libavformat"<<avformat_version();
    logOs<<"libswscale"<<swscale_version();
    logOs<<"libswresample"<<swresample_version();
    logOs<<"===== options =====\n";
//    logOs<<"player-opts"<<player_opts;
//    logOs<<"format-opts"<<format_opts;
//    logOs<<"codec-opts "<<codec_opts;
//    logOs<<"sws-opts   "<<sws_dict;
//    logOs<<"swr-opts   "<<swr_opts;
    logOs<< "===================\n";
    LOGD("%s",logOs.str().c_str());
}

int MyFFPlayer::stop() {
    //todo
    return 0;
}

int MyFFPlayer::pause(bool pause) {
    //todo
    return 0;
}

int MyFFPlayer::seek(long pos) {
    //todo
    return 0;
}
void MyFFPlayer::buffering(bool isBuffering) {
    //todo

}

void MyFFPlayer::checkbuffering() {

}
