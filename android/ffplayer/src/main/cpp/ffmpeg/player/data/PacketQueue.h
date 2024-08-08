//
// Created by Administrator on 2024/7/27.
//

#ifndef JUSTPLAYER_PACKETQUEUE_H
#define JUSTPLAYER_PACKETQUEUE_H


#include "FFinc.h"
#include "data/temp/MyAVPacketList.h"



static AVPacket *flush_pkt = av_packet_alloc();
// 待解码包队列
class PacketQueue {

public:
    PacketQueue(bool abort_request = true);
    ~PacketQueue();


    MyAVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    int64_t duration;
    int abort_request;
    int serial;
//    SDL_mutex *mutex;
//    SDL_cond *cond;
    MyAVPacketList *recycle_pkt;
    int recycle_count;
    int alloc_count;

    bool is_buffer_indicator;

    int put(AVPacket *pkt);
    int put_private(AVPacket *pkt);
    int put_nullpacket(AVPacket *pkt, int stream_index);
    int hasEnoughPackets(AVStream *st, int stream_id, int minFrames);



    void flush();
    void abort();
    void start();
    /* return null if aborted or no packet and > 0 if packet.  */
    AVPacket* get(int block, int *serial);


    bool isFlushPacket(AVPacket *pkt);

};


#endif //JUSTPLAYER_PACKETQUEUE_H
