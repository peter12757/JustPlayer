//
// Created by Administrator on 2023/10/24 0024.
//

#ifndef JUSTPLAYER_IJUSTPLAYER_H
#define JUSTPLAYER_IJUSTPLAYER_H
#include "../const/constants.h"
#include "JustData.h"

class IJustPlayer {
private:


public:
    IJustPlayer(JustContext *ctx);
    ~IJustPlayer();

    virtual RET setDataSource(const string& uri)=0;

    virtual RET prepareAsyc()=0;

    virtual RET setSurface()=0;

    virtual RET start()=0;

    virtual RET pause()=0;

    virtual RET stop()=0;

    virtual RET reset()=0;




};

#endif //JUSTPLAYER_IJUSTPLAYER_H
