#include "JAudioPlayer.h"
#include <mutex>
#include <QObject>

class SAudioPlayer : public JAudioPlayer
{
    Q_OBJECT
public:

    QAudioOutput *output = NULL;
    QIODevice *io = NULL;
    std::mutex mux;

    virtual bool Open() {
        mux.lock();
        QAudioFormat format;
        format.setSampleRate(samplrRate);
        format.setSampleFormat(sampleFormat);
        format.setChannelCount(2);
        if(!device) {
            device =  new  QAudioDevice(QMediaDevices::defaultAudioOutput());
        }
        if (!device->isFormatSupported(format)) {
            qDebug() << "Raw audio format not supported by backend, cannot play audio.";
            return false;
        }
        output = new QAudioOutput(device,this);




        audioSink = new QAudioSink(format,nullptr);

        return false;
    }


    bool playAudioData(const char *data,int64_t len) {

        return false;
    }


    bool Close() {


        return false;
    }

};

JAudioPlayer *JAudioPlayer::getIntance()
{
    static SAudioPlayer player;
    return &player;
}

JAudioPlayer::JAudioPlayer() {}
