#ifndef IJVIDEOCALL_H
#define IJVIDEOCALL_H

#include "../ffinc.h"

class IJVideoCall
{
public:
    virtual void Init(int width,int height) = 0;
    virtual void Repaint(AVFrame *frame) = 0;
};

#endif // IJVIDEOCALL_H
