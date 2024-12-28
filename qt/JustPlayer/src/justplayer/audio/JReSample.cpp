#include "JResample.h"

JResample::JResample() {
    //音频重采样  上下文初始化
    actx = swr_alloc();
}

JResample::~JResample()
{

}

bool JResample::Open(AVCodecParameters *para,bool isClearPara)
{
    if (!para)return false;
    mux.lock();
    int ret = 0;

    AVFilterGraph *graph = avfilter_graph_alloc();
    avfilterctx = avfilter_graph_alloc_filter(graph, avfilter_get_by_name("buffersink"),
                                              "ffplay_buffersink");
    //初始化audiopara
    audio_params.fmt = AV_SAMPLE_FMT_S16;
    audio_params.freq = 44100;
    audio_params.ch_layout = AV_CHANNEL_LAYOUT_STEREO;
    //如果actx=null的话会分配空间

    ret = swr_alloc_set_opts2(&actx,
                              &audio_params.ch_layout,	//输出格式
                              audio_params.fmt,			//输出样本格式 1 AV_SAMPLE_FMT_S16
                              audio_params.freq,					//输出采样率
                              &para->ch_layout,//输入格式
                              (AVSampleFormat)para->format,
                              para->sample_rate,
                              0, NULL
                              );
    qDebug() << "swr_alloc_set_opts2 ret:" << ret;
    qDebug() << "swr_alloc_set_opts2 para->ch_layout:" << &para->ch_layout;
    qDebug() << "swr_alloc_set_opts2 para->format:" << para->format;
    qDebug() << "swr_alloc_set_opts2 para->sample_rate:" << para->sample_rate;

    if(isClearPara)
        avcodec_parameters_free(&para);
    AVChannelLayout *out_ch_layout = nullptr;
    int64_t osf =0;
    int64_t osr =0;
    AVChannelLayout *ichl = nullptr;
    int64_t isf =0;
    int64_t isr =0;

    av_opt_get_chlayout(&actx,"ochl",0,out_ch_layout);
    av_opt_get_int(&actx,"osf",0,&osf);
    av_opt_get_int(&actx,"osr",0,&osf);
    av_opt_get_chlayout(&actx,"ichl",0,ichl);
    av_opt_get_int(&actx,"isf",0,&isf);
    av_opt_get_int(&actx,"isr",0,&isr);

    qDebug() << "swr_alloc_set_opts2 ochl:" << out_ch_layout;
        qDebug() << "swr_alloc_set_opts2 osf:" << osf;
    qDebug() << "swr_alloc_set_opts2 osr:" << osr;
        qDebug() << "swr_alloc_set_opts2 ichl:" << ichl;
    qDebug() << "swr_alloc_set_opts2 isf:" << isf;
        qDebug() << "swr_alloc_set_opts2 isr:" << isr;
    int re = swr_init(actx);
    mux.unlock();
    if (re != 0)
    {
        char buf[1024] = { 0 };
        av_strerror(re, buf, sizeof(buf) - 1);
        qDebug() << "swr_init  failed! :" << buf;
        return false;
    }
    return true;
}

void JResample::Close()
{
     mux.lock();

     mux.unlock();

}

int JResample::Resample(AVFrame *frame, unsigned char *data)
{
    if(!frame || !data) {
        if (frame) {
            av_frame_free(&frame);
        }
        qDebug()<<"Resample fail  !frame || !data";
        return 0;
    }
    uint8_t *dataTmp[2] = { 0 };
    dataTmp[0] = data;
    //frame->nb_samples参数还需要看一下，这里先保持和frame一致
    int re = swr_convert(actx,
                         dataTmp, frame->nb_samples,		//输出
                         frame->extended_data, frame->nb_samples	//输入
                         );
    if (re <= 0) {
        qDebug()<<"Resample fail "<< frame->nb_samples<<"  "<<re;
        return re;
    }
    int outSize = re * frame->ch_layout.nb_channels * av_get_bytes_per_sample(audio_params.fmt);
    return outSize;
}


































