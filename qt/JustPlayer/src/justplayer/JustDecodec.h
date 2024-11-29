#ifndef JUSTDECODEC_H
#define JUSTDECODEC_H


#include "ffinc.h"
#include <QDebug>
#include <mutex>

struct AVCodecParameters;
struct AVCodecContext;
struct AVFrame;
struct AVPacket;

class JustDecodec
{
public:
    JustDecodec();
    virtual ~JustDecodec();

    //打开解码器
    virtual bool Open(AVCodecParameters *para);

    //发送到解码线程，不管是否成功都会释放para的控件（对象和媒体内容）
    virtual bool Send(AVPacket *pkt);

    //获取解码数据，一次send需要多次recv,获取缓冲中的数据send null在recv多次
    //每次复制一份，由调用者释放a_frame_free
    virtual AVFrame* Recv();

    virtual void Close();
    virtual void Clear();


public:
    bool isAudio = false;
    AVCodecContext *codec = 0;
    std::mutex mux;
};

#endif // JUSTDECODEC_H
