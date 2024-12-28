#ifndef TESTCASE_H
#define TESTCASE_H

#include <string>
#include <QThread>
#include <iostream>
#include "justplayer/ffinc.h"
#include "justplayer/JDecodec.h"
#include "justplayer/JDemux.h"
#include "XVideoWidget.h"
#include "justplayer/audio/JResample.h"
#include "justplayer/audio/JAudioPlayer.h"
using namespace std;

class TestThread :public QThread
{
public:
    void Init()
    {
        //�������
        // char *url = "rtmp://58.200.131.2:1935/livetv/hunantv";
        char *url = "test_2.mp4";
        cout << "demux.Open = " << demux.Open(url);
        demux.Read();
        demux.Flush();
        demux.Close();

        cout << "demux.Open = " << demux.Open(url);
        cout << "CopyVPara = " << demux.CopyVPara() << endl;
        cout << "CopyAPara = " << demux.CopyAPara() << endl;
        //cout << "seek=" << demux.Seek(0.95) << endl;

        /////////////////////////////

        cout << "vdecode.Open() = " << vdecode.Open(demux.CopyVPara()) << endl;
        //vdecode.Clear();
        //vdecode.Close();
        cout << "adecode.Open() = " << adecode.Open(demux.CopyAPara()) << endl;
        resample.Open(demux.CopyAPara());
        audio_player = new JAudioPlayer(demux.sampleRate,demux.channels);
        audio_player->Open();

    }
    uint8_t *pcm = new unsigned char[1024 * 1024];
    void run()
    {
        for (;;)
        {
            AVPacket *pkt = demux.Read();
            if (demux.isAudio(pkt))
            {
                adecode.Send(pkt);
                AVFrame *frame = adecode.Recv();

                int len = resample.Resample(frame,pcm);
                while( len>0) {
                    if(audio_player->getFree() >= len){
                        audio_player->write((char *)pcm,len);
                        break;
                    }
                    msleep(1);
                }

                qDebug()<<"Resample:"<<len<<" ";
            }
            else
            {
                vdecode.Send(pkt);
                AVFrame *frame = vdecode.Recv();
                video->Repaint(frame);
                msleep(40);
                //cout << "Video:" << frame << endl;
            }
            if (!pkt)break;
        }
    }
    ///����XDemux
    JDemux demux;
    ///�������
    JDecodec vdecode;
    JDecodec adecode;
    XVideoWidget *video;
    JResample resample;
    JAudioPlayer *audio_player;

};


struct TestCase
{
    std::string filename;
    int video_width; //视频分辨率宽
    int video_height; //视频分辨率高
    TestCase(char *name,int width,int height) {}
};

const static TestCase case1 = TestCase((char*)"encode_yuv_1366_768_yuv444_Frame2.yuv",1366,768);
const static TestCase case2 = TestCase((char*)"case_716_1280_yuv_420.yuv",716,1280);

#endif // TESTCASE_H
