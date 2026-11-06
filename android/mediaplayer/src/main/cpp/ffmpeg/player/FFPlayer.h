//
// Created by Administrator on 2026/10/26.
//

#ifndef JUSTPLAYER_FFPLAYER_H
#define JUSTPLAYER_FFPLAYER_H


#include "data/FFContext.h"
#include "stream/SourceReader.h"
#include "stream/VideoThread.h"
#include "stream/AudioThread.h"
#include "stream/SubtitleThread.h"
#include <string>

using namespace std;

class FFPlayer {
protected:
    FFContext *ctx;
    SourceReader *sourceReader;
    VideoThread *videoThread;
    AudioThread *audioThread;
    SubtitleThread *subtitleThread;


public:
    FFPlayer();
    virtual ~FFPlayer();


    virtual int getState();

    virtual int setDataSource(string source);

    virtual int prepare_async();

    virtual int start();

    virtual int reset();

    virtual int seek(long pos);

    virtual int getPosition();

    virtual int stop();

    virtual int pause();

    virtual int setSurface(ANativeWindow *native_window);
};


#endif //JUSTPLAYER_FFPLAYER_H
