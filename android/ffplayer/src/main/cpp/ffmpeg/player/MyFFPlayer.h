//
// Created by Administrator on 2024/8/4.
//

#ifndef JUSTPLAYER_MYFFPLAYER_H
#define JUSTPLAYER_MYFFPLAYER_H

#include "data/video/VideoObj.h"
#include <jni.h>
#include "../../base/const/common.h"
#include "../../base/log/LogUtil.h"
#include <string>
#include <sstream>
#include <iostream>
#include "data/FFMsg.h"
#include "data/MessageQueue.h"
#include "FFinc.h"
#include "data/audio/AudioObj.h"
#include "VideoState.h"
#include "ReadThread.h"

enum PLAYER_STATE {
    IDLE,INITIALIZED,PREPARING,PREPARED,STARTED,PAUSED,COMPLETED,STOPPED,ERROR
};

class VideoState;
class ReadThread;
class MyFFPlayer {

public:

    const AVClass *av_class;
    VideoObj *videObj;
    AudioObj *audioObj;
    VideoState *mediaState;

    int av_sync_type;
    int start_on_prepared;

    PLAYER_STATE mp_state = IDLE;
    std::string data_source;


    //消息通知线程，需要在queue钟实现线程 todo
    MessageQueue *msg_queue;

    //readthread
    ReadThread *readthread;

#if CONFIG_AVFILTER
    const char **vfilters_list;
    int nb_vfilters;
    char *afilters;
    char *vfilter0;
#endif

    //config
    int pictq_size= VIDEO_PICTURE_QUEUE_SIZE_DEFAULT;


    MyFFPlayer();
    ~MyFFPlayer();

    int setSurface(ANativeWindow *native_window);


    int setDataSource(std::string pString);


    int prepare_async();
    int stop();
    int pause(bool pause);
    int seek(long pos);

private:
    void changeState(PLAYER_STATE state);

    void notifyMessage(int what,int arg1 = INT32_MIN,int arg2 = INT32_MIN,void *obj = nullptr,int obj_len =0);

    void printVersion();
};


#endif //JUSTPLAYER_MYFFPLAYER_H
