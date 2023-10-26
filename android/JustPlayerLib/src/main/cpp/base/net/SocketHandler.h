//
// Created by Peter Xi on 2022/5/21.
//

#ifndef ANDROIDTEST_SOCKETHANDLER_H
#define ANDROIDTEST_SOCKETHANDLER_H

#include "common.h"

namespace NET_SOCKET{

    class SocketBase;

    class SocketHandler
    {
    public:
        virtual int SocketRead(SocketBase*) = 0;
        virtual int SocketWrite(SocketBase*) =0; //only for list socket;
        virtual int onDataRecv(const char*, int, SocketBase*) = 0;
        virtual int onClose(SocketBase*) = 0;
        virtual void onConnected(SocketBase*) = 0;
        virtual void insertGlobalRecvPort(uint32_t port, uint64_t lastTimeStamp) {}
        virtual void eraseGlobalRecvPort(uint32_t port) {}
        virtual ~SocketHandler() {}
    };
}

#endif //ANDROIDTEST_SOCKETHANDLER_H
