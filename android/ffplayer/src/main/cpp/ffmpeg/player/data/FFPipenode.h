//
// Created by Administrator on 2024/7/28.
//

#ifndef JUSTPLAYER_FFPIPENODE_H
#define JUSTPLAYER_FFPIPENODE_H

#include "FFinc.h"
#include "common.h"
#include "LogUtil.h"


class FFPipenode {
public:
//    SDL_mutex *mutex;
    void *opaque;

    FFPipenode(int opaque_size);
    ~FFPipenode();

    virtual void func_destroy() =0;
    virtual int  func_run_sync() = 0;
    virtual int  func_flush() =0 ; // optional

};


#endif //JUSTPLAYER_FFPIPENODE_H
