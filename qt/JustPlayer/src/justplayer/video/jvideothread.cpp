#include "jvideothread.h"

JVideoThread::JVideoThread() {}

JVideoThread::~JVideoThread()
{
    isExited = true;
    wait();
}

void JVideoThread::run()
{
    while (!isExited) {
        mux.lock();
        if(pkt_list.empty() && isAviliable()) {
            mux.unlock();
            msleep(1);
            continue;
        }
        AVPacket *pkt = pkt_list.front();
        pkt_list.pop_back();
        bool ret = video_decodec->Send(pkt);
        if (!ret) {
            mux.unlock();
            msleep(1);
            continue;
        }
        while (AVFrame *frame = video_decodec->Recv()) {

        }

        mux.unlock();

    }
}

bool JVideoThread::Open(AVCodecParameters *para)
{
    if(!para) return false;
    mux.lock();
    if(!video_decodec) {
        video_decodec = new JDecodec();
    }
    if(!video_decodec->Open(para)) {

    }

    return ret;
}

void JVideoThread::push(AVPacket *pkt)
{
    if(!pkt) {
        qDebug()<<"push pkt is null";
        return;
    }
    while (!isExited)
    {
        mux.lock();
        if (pkt_list.size() < maxList)
        {
            pkt_list.push_back(pkt);
            mux.unlock();
            break;
        }
        mux.unlock();
        msleep(1);
    }
}

bool JVideoThread::isAviliable()
{

}




