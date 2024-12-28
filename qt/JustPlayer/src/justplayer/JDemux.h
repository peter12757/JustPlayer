#ifndef JDemux_H
#define JDemux_H

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
class JDemux
{
public:
    JDemux();
    virtual ~JDemux();


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

    //video
    int width = 0;
    int height = 0;
    int totalMs = 0;

    //audio
    int sampleRate = 0;
    int sampleSize = 0;
    int channels = 0;
};

#endif // JDemux_H
