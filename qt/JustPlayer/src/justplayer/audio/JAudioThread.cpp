#include "JAudioThread.h"

JAudioThread::JAudioThread() {}

JAudioThread::~JAudioThread()
{

}

void JAudioThread::run()
{

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
    pkt_list.push_back((pkt));

}
