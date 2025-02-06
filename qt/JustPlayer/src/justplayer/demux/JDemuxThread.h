#ifndef JDEMUXTHREAD_H
#define JDEMUXTHREAD_H

#include <QThread>

#include <justplayer/JDemux.h>


#include <justplayer/audio/JAudioThread.h>

#include <justplayer/video/JVideoThread.h>

#include "../video/IJVideoCall.h"
#include "../ffinc.h"


class JDemuxThread: public QThread
{
public:
    JDemuxThread();
    virtual ~JDemuxThread();

    virtual bool Open(const char *url,IJVideoCall *call);

    virtual void start();

    void run();

public:
    bool isExit = false;
    std::mutex mux;
    JDemux *demux = nullptr;
    JVideoThread *video_thread = nullptr;
    JAudioThread *audio_thread = nullptr;

};

#endif // JDEMUXTHREAD_H
