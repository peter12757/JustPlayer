//
// Created by Administrator on 2024/7/27.
//

#ifndef JUSTPLAYER_PACKETQUEUE_H
#define JUSTPLAYER_PACKETQUEUE_H

#include "FFinc.h"

// 待解码包队列
class PacketQueue {

public:
    PacketQueue();
    ~PacketQueue();


    AVFifoBuffer *pkt_list{};
    int nb_packets{};
    int size{};
    int64_t duration{};
    int abort_request;
    int serial{};

    int packet_queue_put(AVPacket *pkt);
    int packet_queue_put_private(AVPacket *pkt);
    int packet_queue_put_nullpacket(AVPacket *pkt, int stream_index);
    int stream_has_enough_packets(AVStream *st, int stream_id);



};


#endif //JUSTPLAYER_PACKETQUEUE_H
