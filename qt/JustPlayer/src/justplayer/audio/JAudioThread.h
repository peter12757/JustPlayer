#ifndef JAUDIOTHREAD_H
#define JAUDIOTHREAD_H

#include <QThread>
#include "../ffinc.h"
#include "../JDecodec.h"
#include "JAudioPlayer.h"
#include "JResample.h"
#include "QDebug"

class JAudioThread : public QThread
{
public:
    JAudioThread();
    virtual ~JAudioThread();


    void run();

    //打开解码器
    virtual bool Open(AVCodecParameters *para);
    virtual void push(AVPacket *pkt);
    virtual bool isAviliable();



public:
    JDecodec *audio_decodec = nullptr;
    JAudioPlayer *audio_player = nullptr;
    JResample * audio_resample = nullptr;
    std::mutex mux;
    int maxList = 100;
    bool isExited =false;
    std::list <AVPacket *> pkt_list;
    unsigned char *pcm;
    long pcm_size = 1024*1024*10;


};

#endif // JAUDIOTHREAD_H
