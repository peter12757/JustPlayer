#ifndef JVIDEOTHREAD_H
#define JVIDEOTHREAD_H

#include "../JDecodec.h"
#include "QDebug"
#include <QThread>
#include "../ffinc.h"

class JVideoThread : public QThread
{
public:
    JVideoThread();
    virtual ~JVideoThread();

    void run();

    //打开解码器
    virtual bool Open(AVCodecParameters *para);
    virtual void push(AVPacket *pkt);
    virtual bool isAviliable();




public:
    JDecodec *video_decodec = nullptr;
    std::mutex mux;
    std::list <AVPacket *> pkt_list;
    bool isExited =false;
    int maxList = 100;


};

#endif // JVIDEOTHREAD_H
