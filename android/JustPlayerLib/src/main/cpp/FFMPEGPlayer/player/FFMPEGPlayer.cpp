//
// Created by Peter Xi on 2022/5/10.
//


#include "FFMPEGPlayer.h"
#include "JniHelper.h"

using namespace JustData;
using namespace std;

FFMPEGPlayer::FFMPEGPlayer()
: XThread("FFMPEGPlayer",100,URGENT){
    m_pHandler = new PlayerEventHandler();
}

FFMPEGPlayer::~FFMPEGPlayer() {
//    ffmpeg_cleanup
    avformat_network_deinit();
    SafeDelete(mJustPlayerCtx);
    SafeDelete(m_pHandler);
}

RET FFMPEGPlayer::setSurface() {
    std::ostringstream logOs;
    logOs<<" FFMPEGPlayer::setSurface ";
    LOGD("%s",logOs.str().c_str());
    return RET_OK;
}

RET FFMPEGPlayer::setDataSource(const std::string& uri) {
    assert(&uri);
    std::ostringstream logOs;
    logOs<<"FFMPEGPlayer::setDataSource: ";
    RET ret = RET_ERR;
    //local must be <1024
    std::string m_play_uri = av_strdup(uri.c_str());
    if (m_play_uri.empty()){
        logOs<<"url is null";
    }else {
        logOs<<m_play_uri.c_str();
        ret = RET_OK;
    }
    mJustPlayerCtx = new VideoState(m_play_uri);
    reader = new LocalDataReader(mJustPlayerCtx);
    LOGD("%s", logOs.str().c_str());
    return ret;
}

RET FFMPEGPlayer::prepareAsyc() {
    std::ostringstream logOs;
    logOs<<"FFMPEGPlayer::prepareAsyc ";
    if (reader) {
        reader->start();
    } else{
        logOs<<"reader == null";
    }
    LOGD("%s",logOs.str().c_str());
    return RET_OK;
}

RET FFMPEGPlayer::start() {
    LOGD("start");
    return RET_OK;
}


void FFMPEGPlayer::changePlayerState(PlayerState state) {
    player_state = state;
    //TODO notify
}

RET FFMPEGPlayer::pause() {
    LOGD("FFMPEGPlayer::pause");
    return RET_OK;
}

RET FFMPEGPlayer::stop() {

//    avformat_close_input(&mFFCtx->av_format_FFCtx);
//
//    /* note: the internal buffer could have changed, and be != avio_FFCtx_buffer */
//    if (mFFCtx->avio_FFCtx)
//        av_freep(&mFFCtx->avio_FFCtx->buffer);
//    avio_context_free(&mFFCtx->avio_FFCtx);
//
//    av_file_unmap(mFFCtx->avio_FFCtx_buffer, mFFCtx->avio_FFCtx_buffer_size);
return RET_OK;
}


RET FFMPEGPlayer::reset() {
    LOGD("FFMPEGPlayer::reset");
    stop();
    return RET_OK;
}

RET FFMPEGPlayer::invoke(Unpack unpack) {
    RET ret = RET_ERR;
    std::ostringstream logOs;
    logOs<<"FFMPEGPlayer::invoke:";
    uint32_t method = unpack.pop_uint32();
    logOs<<" method: "<<method;
    switch (method) {
        case METHOD_PAUSE: {
            ret = pause();
            break;
        }
        case METHOD_SEEK: {
            uint32_t progress = unpack.pop_uint32();
            //todo
            break;
        }
        case METHOD_GET_DURATION: {

            break;
        }
        case METHOD_GET_POSITION:{
            break;
        }
        case METHOD_PREPARE: {
            LOGD("%s",logOs.str().c_str());
            prepareAsyc();
            break;
        }
        case METHOD_RESET:{
            reset();
            break;
        }
        case METHOD_SET_DATA_SOURCE:{
            std::string source = unpack.pop_varstr();
            logOs<<" uri: "<<source;
            setDataSource(source);
            break;
        }
        case METHOD_START: {
            start();
            break;
        }
        case METHOD_STOP: {
            stop();
            break;
        }
        default:{
            logOs <<"unsupport method";
            LOGD("UNKNOW METHOD CALL :%d",method);
            break;
        }
    }
    LOGD("%s \n",logOs.str().c_str());
    return ret;
}

void FFMPEGPlayer::setCallBack(JNIEnv *pEnv, jobject jcallback) {
    std::ostringstream logOs;
    logOs<<"FFMPEGPlayer::setCallBack";
    if (this->jCallBack) {
        pEnv->DeleteGlobalRef(this->jCallBack);
        if (!jCallBackMethodID) {
            jclass javaClass = pEnv->GetObjectClass(jCallBack);
            jCallBackMethodID = pEnv->GetMethodID(javaClass,
                                                    "onCallBack", "([B)[B");
        }
    }
    if (jcallback) this->jCallBack = jcallback;
    LOGD("%s",logOs.str().c_str());
}

void FFMPEGPlayer::onCreate() {
    std::ostringstream logOS;
    logOS<<"FFMPEGPlayer::onCreate() ";
    LOGD("%s",logOS.str().c_str());
}

void FFMPEGPlayer::onThreadRun(uint32_t now) {
    std::ostringstream logOS;
    logOS<<"FFMPEGPlayer::onThreadRun() ";
    m_pHandler->onEventRun();
    LOGD("%s",logOS.str().c_str());
}

jobject FFMPEGPlayer::callJavaMethod(string data) {
    std::ostringstream log;
    log<<"FFMPEGPlayer::callJavaMethod data:"<<data.c_str();
    JNIEnv *env = JniHelper::attachJVM();
    jbyteArray result;
    jobject obj = env->CallObjectMethod(jCallBack, jCallBackMethodID,
                                                                   JniHelper::str2jbyteArray(env,
                                                                                             data),
                                                                   result);
    log<<" callbakc data: "<<&obj;
    LOGD("%s",log.str().c_str());
    return env->NewGlobalRef(obj);

}





