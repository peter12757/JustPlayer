//
// Created by Peter Xi on 2022/5/10.
//

#ifndef ANDROIDTEST_JUSTPLAYER_H
#define ANDROIDTEST_JUSTPLAYER_H

#include "../../base/util/LogUtil.h"
#include "../../base/config/constants.h"
#include "../../base/util/protocol/package.h"
#include "../../JustData.h"
#include <sstream>
#include <jni.h>
#include "../../util/statemachine/StateMachine.h"

#include "../../base/util/Util.h"
#include "data/VideoState.h"
#include "LocalDataReader.h"
#include "PlayerEventHandler.h"

using namespace std;

class AudioBaseEngine;
class VideoState;
class LocalDataReader;

enum  PlayerState{
    IDLE,
    INIT,
    PREPARING,
    PREPARED,
    START,
    PAUSE,
    STOPPING,
    STOPPED,
};




class FFMPEGPlayer : public XThread{
    //data
protected:
    VideoState* mJustPlayerCtx;
    LocalDataReader *reader;
    PlayerState player_state = IDLE;
    jobject jCallBack;
    jmethodID jCallBackMethodID;

    AudioBaseEngine *audioEngine;

    //thread
    virtual void onThreadRun(uint32_t now);

public:
    FFMPEGPlayer();
    ~FFMPEGPlayer();


    RET setDataSource(const string& uri);

    RET prepareAsyc();

    RET setSurface();

    RET start();

    RET pause();

    RET stop();

    RET reset();

    void changePlayerState(PlayerState state);

    RET invoke(Unpack unpack);
    void setCallBack(JNIEnv *pEnv, jobject jcallback);

    jobject callJavaMethod(string data);




    //ffmpeg api


    //thread
    virtual void onCreate();


protected:
    PlayerEventHandler* m_pHandler;

};


#endif //ANDROIDTEST_JUSTPLAYER_H