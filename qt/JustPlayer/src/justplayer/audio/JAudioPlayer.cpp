#include "JAudioPlayer.h"


JAudioPlayer::JAudioPlayer(int samplrRate,int channels)
    :samplrRate(samplrRate)
    ,channels(channels){

}

JAudioPlayer::~JAudioPlayer()
{

}

bool JAudioPlayer::Open()
{

    //配置audio format

    QAudioFormat format;
    format.setSampleRate(samplrRate);
    format.setSampleFormat(sampleFormat);
    format.setChannelCount(2);
    //在qt6.0之后会有一些改变，去掉了几个接口
    // format.setByteOrder(QAudioFormat::LittleEndian);
    // format.setCodec(QLatin1String("audio/pcm"));
    // format.setSampleSize(8);
    // format.setSampleType(QAudioFormat::UnSignedInt);
    Close();
    mux.lock();
    if(!device) {
        device =  new QAudioDevice(QMediaDevices::defaultAudioOutput());
    }
    if (!device->isFormatSupported(format)) {
        qDebug() << "Raw audio format not supported by backend, cannot play audio.";
        return false;
    }
    audioSink = new QAudioSink(*device,format);

    io = audioSink->start();
    mux.unlock();
    if(io) {
        return true;
    }
    return false;
}

bool JAudioPlayer::Close()
{
    mux.lock();
    if(audioSink) {
        io->close();
        audioSink->stop();
        delete audioSink;
        audioSink = nullptr;
        io = nullptr;
    }
    mux.unlock();

}

void JAudioPlayer::serVolume(qreal v)
{

}

qreal JAudioPlayer::volume() const
{

}

int JAudioPlayer::setBufferSize(int bytes)
{

}

void JAudioPlayer::setChannelConfig(QAudioFormat::ChannelConfig)
{

}

QAudioFormat::ChannelConfig JAudioPlayer::channelConfig() const
{

}

bool JAudioPlayer::write(const char *data, int dataSize)
{
    if(!data ||dataSize <=0) {
        return false;
    }
    mux.lock();
    if(!audioSink || !io) {
        mux.unlock();
        return false;
    }
    int size = io->write(data,dataSize);
    mux.unlock();
    if(size != dataSize) {
        return false;
    }
    return true;
}

int JAudioPlayer::getFree()
{
    mux.lock();
    if(!audioSink) {
        mux.unlock();
        return 0;
    }
    int free = audioSink->bytesFree();
    mux.unlock();
    return free;
}
