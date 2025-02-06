#include "JDemuxThread.h"

JDemuxThread::JDemuxThread() {}

JDemuxThread::~JDemuxThread()
{

}

bool JDemuxThread::Open(const char *url, IJVideoCall *call)
{
    if (url == 0 || url[0] == '\0')
        return false;

    mux.lock();
    if (!demux) demux = new JDemux();
    if (!video_thread) video_thread = new JVideoThread();
    if (!audio_thread) audio_thread = new JAudioThread();


    bool re = demux->Open(url);
    if (!re)
    {
        cout << "demux->Open(url) failed!" << endl;
        return false;
    }

    if (!video_thread->Open(demux->CopyVPara(), call, demux->width, demux->height))
    {
        re = false;
        cout << "vt->Open failed!" << endl;
    }

    if (!audio_thread->Open(demux->CopyAPara(), demux->sampleRate, demux->channels))
    {
        re = false;
        cout << "at->Open failed!" << endl;
    }
    mux.unlock();
    cout << "XDemuxThread::Open " << re << endl;
    return re;
}

void JDemuxThread::start()
{

}

void JDemuxThread::run()
{
    while (!isExit) {
        mux.lock();
        if(!demux) {
            mux.unlock();
            msleep(5);
            continue;
        }
        AVPacket *pkt = demux->Read();
        if(!pkt) {
            mux.unlock();
            msleep(5);
            continue;
        }
        if(demux->isAudio(pkt)) {
            if(audio_thread) {
                audio_thread->push(pkt);
            }else if(video_thread) {
                video_thread->push(pkt);
            }
        }
        mux.lock();
    }
}
