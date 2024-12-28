#ifndef JAUDIOPLAYER_H
#define JAUDIOPLAYER_H


#include "../ffinc.h"
#include <QtMultimedia/QAudioFormat>
#include <QtMultimedia/QAudioOutput>
#include <QtMultimedia/QAudioDevice>
#include <QtMultimedia/QMediaDevices>
#include <QtMultimedia/QAudioSink>
#include <QDebug>
#include <mutex>
#include <QObject>

class JAudioPlayer
{
public:

    JAudioPlayer(int samplrRate = 44100,int channels =2);
    virtual ~JAudioPlayer();

    //打开音频播放
    virtual bool Open();
    //关闭音频播放
    virtual bool Close();

    //播放音频数据
    virtual void serVolume(qreal v);
    virtual qreal volume() const;
    virtual int setBufferSize(int bytes);
    virtual void setChannelConfig(QAudioFormat::ChannelConfig);
    QAudioFormat::ChannelConfig channelConfig() const;
    //写入音频数据
    virtual bool write(const char *data,int dataSize);
    virtual int getFree();





public:
    //audioformat config
    int samplrRate = 44100;
    QAudioFormat::SampleFormat sampleFormat = QAudioFormat::SampleFormat::Int16;
    int channels = 2;
    QAudioFormat *format;

    //在qt6.0之后使用audiosink
    // QAudioOutput *output;
    QAudioSink *audioSink;
    QAudioDevice *device;


    QIODevice *io = NULL;


private:
    std::mutex mux;





};

#endif // JAUDIOPLAYER_H
