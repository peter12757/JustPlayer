//
// Created by Peter Xi on 2022/5/10.
//


#include "JustPlayer.h"
#include "JniHelper.h"

using namespace JustData;
using namespace std;

JustPlayer::JustPlayer()
: XThread("JustPlayer",100,URGENT){
    m_pHandler = new PlayerEventHandler();
}

JustPlayer::~JustPlayer() {
//    ffmpeg_cleanup
    avformat_network_deinit();
    SafeDelete(mJustPlayerCtx);
    SafeDelete(m_pHandler);
}

RET JustPlayer::setSurface() {
    std::ostringstream logOs;
    logOs<<" JustPlayer::setSurface ";
    LOGD("%s",logOs.str().c_str());
    return RET_OK;
}

RET JustPlayer::setDataSource(const std::string& uri) {
    assert(&uri);
    std::ostringstream logOs;
    logOs<<"JustPlayer::setDataSource: ";
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

RET JustPlayer::prepareAsyc() {
    std::ostringstream logOs;
    logOs<<"JustPlayer::prepareAsyc ";
    if (reader) {
        reader->start();
    } else{
        logOs<<"reader == null";
    }
    LOGD("%s",logOs.str().c_str());
    return RET_OK;
}

RET JustPlayer::start() {
    LOGD("start");
    return RET_OK;
}


void JustPlayer::changePlayerState(PlayerState state) {
    player_state = state;
    //TODO notify
}

RET JustPlayer::pause() {
    LOGD("JustPlayer::pause");
    return RET_OK;
}

RET JustPlayer::stop() {

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


RET JustPlayer::reset() {
    LOGD("JustPlayer::reset");
    stop();
    return RET_OK;
}

RET JustPlayer::invoke(Unpack unpack) {
    RET ret = RET_ERR;
    std::ostringstream logOs;
    logOs<<"JustPlayer::invoke:";
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

void JustPlayer::setCallBack(JNIEnv *pEnv, jobject jcallback) {
    std::ostringstream logOs;
    logOs<<"JustPlayer::setCallBack";
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

void JustPlayer::onCreate() {
    std::ostringstream logOS;
    logOS<<"JustPlayer::onCreate() ";
    LOGD("%s",logOS.str().c_str());
}

void JustPlayer::onThreadRun(uint32_t now) {
    std::ostringstream logOS;
    logOS<<"JustPlayer::onThreadRun() ";
    m_pHandler->onEventRun();
    LOGD("%s",logOS.str().c_str());
}

jobject JustPlayer::callJavaMethod(string data) {
    std::ostringstream log;
    log<<"JustPlayer::callJavaMethod data:"<<data.c_str();
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





