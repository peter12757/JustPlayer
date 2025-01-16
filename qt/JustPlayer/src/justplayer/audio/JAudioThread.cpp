#include "JAudioThread.h"

JAudioThread::JAudioThread() {
    pcm = new unsigned char[pcm_size];  //pc端
}

JAudioThread::~JAudioThread()
{
    delete pcm;
    pcm = nullptr;
}

void JAudioThread::run()
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
        bool ret = audio_decodec->Send(pkt);
        if (!ret) {
            mux.unlock();
            msleep(1);
            continue;
        }
        while (AVFrame *frame = audio_decodec->Recv()) {
            memset(pcm,0,pcm_size);
            //重采样
            int size = audio_resample->Resample(frame,pcm);
            //播放
            if( audio_player->getFree() <size) {
                msleep(1);
                continue;
            }
            audio_player->write((const char *)pcm,size);

        }

        mux.unlock();

    }

}

bool JAudioThread::Open(AVCodecParameters *para)
{
    if(!para) return false;
    mux.lock();
    if(!audio_decodec) {
        audio_decodec = new JDecodec();
    }
    if(!audio_resample) {
        audio_resample = new JResample();
    }
    if(!audio_player) {
        audio_player = new JAudioPlayer(para->sample_rate,para->ch_layout.nb_channels);
    }
    if(!audio_decodec || !audio_resample || !audio_player) return false;
    bool ret = audio_resample->Open(para);
    if(!ret) {
        qDebug()<<"audio_resample open fail";
    }
    ret = audio_player->Open();
    if(!ret) {
        qDebug()<<"audio_player open fail";
    }
    ret = audio_decodec->Open(para);
    if(!ret) {
        qDebug()<<"audio_player open fail";
    }
    mux.unlock();
    return ret;
}

void JAudioThread::push(AVPacket *pkt)
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

bool JAudioThread::isAviliable()
{
    return audio_decodec && audio_resample && audio_player;
}
