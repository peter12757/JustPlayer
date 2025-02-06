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
        if(pkt_list.empty() || !video_decodec) {
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
            if(isExited || !frame) {
                if(videocall) {
                    videocall->Repaint(frame);
                }
            }
        }
        mux.unlock();

    }
}

bool JVideoThread::Open(AVCodecParameters *para,IJVideoCall *call,int width,int height)
{
    if(!para) return false;
    int ret = true;
    mux.lock();
    videocall = call;
    if(!call) {
        qDebug()<<"videocall == null";
        ret = false;
    }
    videocall->Init(width,height);
    if(!video_decodec) {
        video_decodec = new JDecodec();
    }
    if(!video_decodec->Open(para)) {
        qDebug()<<"video_decodec open fail";
        ret = false;
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




