#ifndef JAUDIOPLAYER_H
#define JAUDIOPLAYER_H


#include "../ffinc.h"
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QAudioDevice>
#include <QtMultimedia/QMediaDevices>
#include <QtMultimedia/QAudioSink>
#include <QtMultimedia/AudioHelp
#include <QDebug>
#include <QObject>

class JAudioPlayer
{
public:

    static JAudioPlayer *getIntance();

    JAudioPlayer();
    virtual ~JAudioPlayer();

    //打开音频播放
    virtual bool Open()=0;
    //关闭音频播放
    virtual bool Close() = 0;

    //播放音频数据
    virtual bool playAudioData(const char *data,int64_t len) = 0;





public:
    int samplrRate = 44100;
    QAudioFormat::SampleFormat sampleFormat = QAudioFormat::SampleFormat::Int16;
    int channels = 2;
    QAudioOutput *output;
    QAudioSink *audioSink;
    QAudioDevice *device;




};

#endif // JAUDIOPLAYER_H
