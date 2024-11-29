#include "JustDemux.h"

// #include <iostream>
// using namespace std;
// extern "C" {
// #include "./../ffmpeg/libavformat/avformat.h"
// }


JustDemux::JustDemux() {
    //初始化封装库
    avformat_network_init();

}

JustDemux::~JustDemux()
{

}




bool JustDemux::Open(const string url)
{

    Close();
    //参数设置
    AVDictionary *opts = NULL;
    //设置resp流使用tcp协议打开
    av_dict_set(&opts, "rtsp_transport", "tcp", 0);

    //网络延时时间
    av_dict_set(&opts, "max_delay", "500", 0);

    mux.lock();
    char *url_ptr = (char*)url.c_str();
    int re = avformat_open_input(
        &ic,
        url_ptr,
        0,  // 0表示自动选择解封器
        &opts //参数设置
        );
    if (re != 0)
    {
        mux.unlock();
        char buf[1024] = { 0 };
        av_strerror(re, buf, sizeof(buf) - 1);
        cout << "open " << url << " failed! :" << buf << endl;
        return false;
    }
    cout << "open " << url << " success! " << endl;

    //获取流信息
    re = avformat_find_stream_info(ic, 0);

    //总时长，ms
    int totalMs = ic->duration / (AV_TIME_BASE / 1000);
    cout << "totalMs = " << totalMs << endl;

    //打印视频流信息
    av_dump_format(ic, 0, url_ptr, 0);


    // 获取视频流
    videoStream = av_find_best_stream(ic, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    AVStream *as = ic->streams[videoStream];
    width = as->codecpar->width;
    height = as->codecpar->height;

    cout << "=======================================================" << endl;
    cout << videoStream << "video stream info" << endl;
    cout << "codec_id = " << as->codecpar->codec_id << endl;
    cout << "format = " << as->codecpar->format << endl;
    cout << "width=" << as->codecpar->width << endl;
    cout << "height=" << as->codecpar->height << endl;
    //帧率fps 分数转换
    cout << "video fps = " << r2d(as->avg_frame_rate) << endl;


    cout << "=======================================================" << endl;
    cout << audioStream << "audio stream info" << endl;
    //获取音频流
    audioStream = av_find_best_stream(ic, AVMEDIA_TYPE_AUDIO, -1, -1, NULL, 0);
    as = ic->streams[audioStream];
    cout << "codec_id = " << as->codecpar->codec_id << endl;
    cout << "format = " << as->codecpar->format << endl;
    cout << "sample_rate = " << as->codecpar->sample_rate << endl;
    //AVSampleFormat;
    cout << "ch_layout = " << as->codecpar->ch_layout.nb_channels << endl;
    //一帧数据，单通道的样本数
    cout << "frame_size = " << as->codecpar->frame_size << endl;
    //1024 * 2 * 2 = 4096  fps = sample_rate/frame_size
    mux.unlock();

    return true;
}

AVPacket *JustDemux::Read()
{
    mux.lock();
    if(!ic) {
        mux.unlock();
        qDebug()<<"ic == null";
        return nullptr;
    }
    AVPacket *pkt = av_packet_alloc();
    int ret = av_read_frame(ic,pkt);
    if(ret != 0) {
        mux.unlock();
        av_packet_free(&pkt);
       qDebug()<<"av_read_frame fail";
        return nullptr;
    }
    pkt->pts = pkt->pts*(1000 * (r2d(ic->streams[pkt->stream_index]->time_base)));
    pkt->dts = pkt->dts*(1000 * (r2d(ic->streams[pkt->stream_index]->time_base)));
    mux.unlock();
    qDebug()<<"pkt->pts: " << pkt->pts;
    return pkt;

}

AVCodecParameters *JustDemux::CopyVPara()
{
    mux.lock();
    if(!ic) {
        mux.unlock();
        qDebug()<<"ic == null";
        return nullptr;
    }
    AVCodecParameters *vpa = avcodec_parameters_alloc();
    avcodec_parameters_copy(vpa,ic->streams[videoStream]->codecpar);


    mux.unlock();
    return vpa;
}

AVCodecParameters *JustDemux::CopyAPara()
{
    mux.lock();
    if(!ic) {
        mux.unlock();
        qDebug()<<"ic == null";
        return nullptr;
    }
    AVCodecParameters *apa = avcodec_parameters_alloc();
    avcodec_parameters_copy(apa,ic->streams[videoStream]->codecpar);

    mux.unlock();
    return apa;

}

bool JustDemux::isAudio(AVPacket *pkt)
{
    if( !pkt) {
        return false;
    }
    if (pkt->stream_index == videoStream)
        return false;
    return true;
}

bool JustDemux::Seek(double pos)
{
    long long seekpos = 0;
    mux.lock();
    if(!ic) {
        mux.unlock();
        qDebug()<<"ic == null";
        return false;
    }
    //清理读取缓冲
    avformat_flush(ic);
    seekpos = ic->streams[audioStream]->duration*pos;
    int ret = av_seek_frame(ic,videoStream,seekpos,AVSEEK_FLAG_FRAME|AVSEEK_FLAG_BACKWARD);

    mux.unlock();
    if(ret <0) {
        return false;
    }
    return true;
}

bool JustDemux::Flush()
{
    mux.lock();
    if(!ic) {
        mux.unlock();
        qDebug()<<"ic == null";
        return false;
    }
    //清理读取缓冲
    avformat_flush(ic);
    mux.unlock();
    return true;
}

bool JustDemux::Close()
{
    mux.lock();
    if(!ic) {
        mux.unlock();
        qDebug()<<"ic == null";
        return false;
    }
    //关闭
    avformat_close_input(&ic);
    totalMs = 0;
    mux.unlock();
    return true;
}
