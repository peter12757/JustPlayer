//
// Created by Administrator on 2024/8/4.
//

#ifndef JUSTPLAYER_MYFFPLAYER_H
#define JUSTPLAYER_MYFFPLAYER_H

#include "data/video/VideoObj.h"
#include <jni.h>
#include "../../base/const/common.h"
#include <string>


class MyFFPlayer {

public:
    VideoObj *videObj;


    MyFFPlayer();
    ~MyFFPlayer();

    int setSurface(ANativeWindow *native_window);
};


#endif //JUSTPLAYER_MYFFPLAYER_H
