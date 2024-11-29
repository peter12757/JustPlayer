#include "JustDecodec.h"

JustDecodec::JustDecodec()
{

}

JustDecodec::~JustDecodec()
{

}

bool JustDecodec::Open(AVCodecParameters *para)
{
    if(!para){
        return false;
    }
    Close();
    //解码器打开
    //找到解码器
    const AVCodec *vcodec = avcodec_find_decoder(para->codec_id);
    if (!vcodec) {
        avcodec_parameters_free(&para);
        qDebug() << "can't find the codec id " << para->codec_id;
        return false;
    }
    qDebug() << "find the AVCodec " << para->codec_id;
    mux.lock();
    codec = avcodec_alloc_context3(vcodec);
    //配置解码器上下文参数
    avcodec_parameters_to_context(codec, para);
    avcodec_parameters_free(&para);

    //8线程解码
    codec->thread_count = 8;

    int ret = avcodec_open2(codec,0,0);
    if (ret != 0) {
        avcodec_free_context(&codec);
        mux.unlock();
        char buf[1024] = {0};
        av_strerror(ret,buf,sizeof(buf)-1);
        qDebug() << " avcodec_open2 fail :" <<buf;
        return false;
    }

    mux.unlock();
    qDebug() << "avcodec_open2 success";
    return true;
}

bool JustDecodec::Send(AVPacket *pkt)
{
    //容错处理
    if (!pkt || pkt->size <= 0 || !pkt->data)return false;
    mux.lock();
    if (!codec)
    {
        mux.unlock();
        return false;
    }
    int re = avcodec_send_packet(codec, pkt);
    mux.unlock();
    av_packet_free(&pkt);
    if (re != 0)return false;
    return true;

}

AVFrame *JustDecodec::Recv()
{
    mux.lock();
    if (!codec)
    {
        mux.unlock();
        return nullptr;
    }
    AVFrame *frame = av_frame_alloc();
    int re = avcodec_receive_frame(codec, frame);
    mux.unlock();
    if (re != 0)
    {
        av_frame_free(&frame);
        return nullptr;
    }
    qDebug() << "["<<frame->linesize[0] << "] ";
    return frame;
}

void JustDecodec::Close()
{
    mux.lock();
    if (codec)
    {
        avcodec_free_context(&codec);
    }
    mux.unlock();
}

void JustDecodec::Clear()
{
    mux.lock();
    //清理解码缓冲
    if (codec)
        avcodec_flush_buffers(codec);

    mux.unlock();
}


