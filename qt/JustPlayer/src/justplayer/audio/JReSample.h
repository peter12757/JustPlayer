#ifndef JResample_H
#define JResample_H

#include "../ffinc.h"
#include <QDebug>
#include <mutex>


typedef struct AudioParams {
    int freq;
    AVChannelLayout ch_layout;
    enum AVSampleFormat fmt;
    int frame_size;
    int bytes_per_sec;
} AudioParams;



class JResample
{
public:
    JResample();
    ~JResample();

    //输出参数和输入参数一致除了采样格式，实处为S16
    virtual bool Open(AVCodecParameters *para,bool isClearPara = false);
    virtual void Close();

    //返回重采样后的大小 不管成功与否都释放frame空间
    virtual int Resample(AVFrame * frame, unsigned char * data);


public:
    std::mutex mux;
    struct SwrContext *actx = nullptr;
    AudioParams audio_params;         // audio filter graph
    AVFilterContext *avfilterctx;
};

#endif // JResample_H
