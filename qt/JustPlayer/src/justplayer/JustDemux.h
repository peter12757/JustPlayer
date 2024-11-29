#ifndef JUSTDEMUX_H
#define JUSTDEMUX_H

#include <stdio.h>
#include <mutex>
#include <iostream>
#include <QDebug>
using namespace std;

#include "ffinc.h"


struct AVFormatContext;
struct AVPacket;
struct AVCodecParameters;

//打开媒体文件，或者流媒体
class JustDemux
{
public:
    JustDemux();
    virtual ~JustDemux();


    virtual bool Open(const string url);

    virtual AVPacket *Read();

    //获取视频参数，返回的指针需要清理，使用avcodex_parameters_free
    AVCodecParameters *CopyVPara();
    //获取音频参数，返回的指针需要清理，使用avcodex_parameters_free
    AVCodecParameters *CopyAPara();

    virtual bool isAudio(AVPacket *pkt);

    //pos 【0,1】 seek的百分比
    virtual bool Seek(double pos);

    virtual bool Flush();
    virtual bool Close();

    static double r2d(AVRational r)
    {
        return r.den == 0 ? 0 : (double)r.num / (double)r.den;
    }


public:
    std::mutex mux;
    //流上下文
    AVFormatContext *ic = NULL;
    //音视频索引，读取时区分音视频
    int videoStream = 0;
    int audioStream = 1;

    int totalMs = 0;

};

#endif // JUSTDEMUX_H
