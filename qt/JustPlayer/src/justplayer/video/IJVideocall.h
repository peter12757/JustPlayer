#ifndef IJVIDEOCALL_H
#define IJVIDEOCALL_H

#include "../ffinc.h"

class IVideoCall
{
public:
    virtual void Init(int width,int height) = 0;
    virtual void Repain(AVFrame *frame) = 0;
};

#endif // IJVIDEOCALL_H
