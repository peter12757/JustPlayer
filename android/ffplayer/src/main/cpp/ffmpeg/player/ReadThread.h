//
// Created by PeterXi on 2024/8/10.
//

#ifndef JUSTPLAYER_READTHREAD_H
#define JUSTPLAYER_READTHREAD_H


#include "data/VideoState.h"
#include "XThread.h"

class ReadThread :XThread("") {ß
public:

    VideoState *vs;

    AVFormatContext *ic;
    int errCode;



    ReadThread(VideoState *is);
    ~ReadThread();



};


#endif //JUSTPLAYER_READTHREAD_H
