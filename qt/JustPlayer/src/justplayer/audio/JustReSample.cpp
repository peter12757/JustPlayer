#include "JustResample.h"

JustResample::JustResample() {
    //音频重采样  上下文初始化
    actx = swr_alloc();
}

JustResample::~JustResample()
{

}

bool JustResample::Open(AVCodecParameters *para,bool isClearPara)
{
    if (!para)return false;
    mux.lock();

    //���actxΪNULL�����ռ�
    const AVChannelLayout* put_ch_layout =  audio_params.ch_layout;

    int ret = swr_alloc_set_opts2(&actx,
                              put_ch_layout,	//输出格式
                              audio_params.fmt,			//输出样本格式 1 AV_SAMPLE_FMT_S16
                              audio_params.freq,					//输出采样率
                              para->ch_layout,//�����ʽ
                              para->format,
                              para->sample_rate,
                              0, NULL
                              );
    if(isClearPara)
        avcodec_parameters_free(&para);
    int re = swr_init(actx);
    mux.unlock();
    if (re != 0)
    {
        char buf[1024] = { 0 };
        av_strerror(re, buf, sizeof(buf) - 1);
        qDebug() << "swr_init  failed! :" << buf;
        return false;
    }
    //unsigned char *pcm = NULL;

    return true;
}

void JustResample::Close()
{
     mux.lock();

     mux.unlock();

}

int JustResample::Resample(AVFrame *frame, unsigned char *data)
{
    if(!frame || !data) {
        if (frame) {
            av_frame_free(&frame);
        }
        return 0;
    }
    uint8_t *dataTmp[2] = { 0 };
    dataTmp[0] = data;
    int re = swr_convert(actx,
                         dataTmp, frame->nb_samples,		//输出
                         (const uint8_t**)frame->data, frame->nb_samples	//输入
                         );
    if (re <= 0)return re;
    int outSize = re * frame->ch_layout.nb_channels * av_get_bytes_per_sample((AVSampleFormat)outFormat);
    return outSize;
}
