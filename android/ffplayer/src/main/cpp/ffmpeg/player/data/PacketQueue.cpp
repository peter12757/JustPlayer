//
// Created by Administrator on 2024/7/27.
//

#include "PacketQueue.h"

PacketQueue::PacketQueue() {
//    memset(this, 0, sizeof(PacketQueue));
//    mutex = SDL_CreateMutex();
//    if (!mutex) {
//        av_log(NULL, AV_LOG_FATAL, "SDL_CreateMutex(): %s\n", SDL_GetError());
//        return AVERROR(ENOMEM);
//    }
//    cond = SDL_CreateCond();
//    if (!cond) {
//        av_log(NULL, AV_LOG_FATAL, "SDL_CreateCond(): %s\n", SDL_GetError());
//        return AVERROR(ENOMEM);
//    }
    abort_request = 1;
}

PacketQueue::~PacketQueue() {
    flush();

//    SDL_LockMutex(mutex);
    while(recycle_pkt) {
        MyAVPacketList *pkt = recycle_pkt;
        if (pkt)
            recycle_pkt = pkt->next;
        av_freep(&pkt);
    }
//    SDL_UnlockMutex(mutex);
//    SDL_DestroyMutex(mutex);
//    SDL_DestroyCond(cond);
}

void PacketQueue::flush() {
    MyAVPacketList *pkt, *pkt1;

//    SDL_LockMutex(mutex);
    for (pkt = first_pkt; pkt; pkt = pkt1) {
        pkt1 = pkt->next;
        av_packet_unref(&pkt->pkt);
#ifdef FFP_MERGE
        av_freep(&pkt);
#else
        pkt->next = recycle_pkt;
        recycle_pkt = pkt;
#endif
    }
    last_pkt = NULL;
    first_pkt = NULL;
    nb_packets = 0;
    size = 0;
    duration = 0;
//    SDL_UnlockMutex(mutex);
}

void PacketQueue::abort() {
//    SDL_LockMutex(mutex);

    abort_request = 1;

//    SDL_CondSignal(cond);

//    SDL_UnlockMutex(mutex);
}

void PacketQueue::start() {
//    SDL_LockMutex(mutex);
    abort_request = 0;
    put_private(flush_pkt);
//    SDL_UnlockMutex(mutex);

}

AVPacket *PacketQueue::get(int block, int *serial) {
    MyAVPacketList *pkt1;
    AVPacket *pkt = nullptr;
    int ret;

//    SDL_LockMutex(mutex);

    for (;;) {
        if (abort_request) {
            ret = -1;
            break;
        }

        pkt1 = first_pkt;
        if (pkt1) {
            first_pkt = pkt1->next;
            if (!first_pkt)
                last_pkt = NULL;
            nb_packets--;
            size -= pkt1->pkt.size + sizeof(*pkt1);
            duration -= pkt1->pkt.duration;
            pkt = &pkt1->pkt;
            if (serial)
                *serial = pkt1->serial;
#ifdef FFP_MERGE
            av_free(pkt1);
#else
            pkt1->next = recycle_pkt;
            recycle_pkt = pkt1;
#endif
            ret = 1;
            break;
        } else if (!block) {
            ret = 0;
            break;
        } else {
//            SDL_CondWait(cond, mutex);
        }
    }
//    SDL_UnlockMutex(mutex);
    return pkt;
}

bool PacketQueue::isFlushPacket(AVPacket *pkt) {
    if (!pkt)
        return false;

    return pkt->data == flush_pkt->data;
}

int PacketQueue::put(AVPacket *pkt) {
    int ret;

//    SDL_LockMutex(mutex);
    ret = put_private(pkt);
//    SDL_UnlockMutex(mutex);

    if (pkt != &flush_pkt && ret < 0)
        av_packet_unref(pkt);

    return ret;
}

int PacketQueue::put_private(AVPacket *pkt) {
    MyAVPacketList *pkt1;

    if (abort_request)
        return -1;

#ifdef FFP_MERGE
    pkt1 = av_malloc(sizeof(MyAVPacketList));
#else
    pkt1 = recycle_pkt;
    if (pkt1) {
        recycle_pkt = pkt1->next;
        recycle_count++;
    } else {
        alloc_count++;
        pkt1 = new MyAVPacketList();
    }
#ifdef FFP_SHOW_PKT_RECYCLE
    int total_count = recycle_count + alloc_count;
    if (!(total_count % 50)) {
        av_log(ffp, AV_LOG_DEBUG, "pkt-recycle \t%d + \t%d = \t%d\n", recycle_count, alloc_count, total_count);
    }
#endif
#endif
    if (!pkt1)
        return -1;
    pkt1->pkt = *pkt;
    pkt1->next = NULL;
    if (pkt == flush_pkt)
        serial++;
    pkt1->serial = serial;

    if (!last_pkt)
        first_pkt = pkt1;
    else
        last_pkt->next = pkt1;
    last_pkt = pkt1;
    nb_packets++;
    size += pkt1->pkt.size + sizeof(*pkt1);
    duration += pkt1->pkt.duration;
    /* XXX: should duplicate packet data in DV case */
//    SDL_CondSignal(cond);
    return 0;
}

int PacketQueue::put_nullpacket(AVPacket *pkt, int stream_index) {
    pkt = av_packet_alloc();
    pkt->data = NULL;
    pkt->size = 0;
    pkt->stream_index = stream_index;
    return put(pkt);
}

int PacketQueue::hasEnoughPackets(AVStream *st, int stream_id,int minFrames) {
    return stream_id < 0 ||
           abort_request ||
           (st->disposition & AV_DISPOSITION_ATTACHED_PIC) ||
           #ifdef FFP_MERGE
           queue->nb_packets > MIN_FRAMES && (!queue->duration || av_q2d(st->time_base) * queue->duration > 1.0);
           #endif
           nb_packets > minFrames;
}










