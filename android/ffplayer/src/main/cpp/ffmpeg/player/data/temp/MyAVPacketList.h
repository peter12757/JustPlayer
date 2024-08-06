//
// Created by Administrator on 2024/7/28.
//

#ifndef JUSTPLAYER_MYAVPACKETLIST_H
#define JUSTPLAYER_MYAVPACKETLIST_H


#include "FFinc.h"

class MyAVPacketList {
public:
    AVPacket pkt;
    int serial;
    MyAVPacketList *next;
};


#endif //JUSTPLAYER_MYAVPACKETLIST_H
