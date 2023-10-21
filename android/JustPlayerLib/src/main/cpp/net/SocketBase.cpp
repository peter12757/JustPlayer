//
// Created by Peter Xi on 2022/5/21.
//

#include "SocketBase.h"

namespace NET_SOCKET{
    std::atomic<uint32_t>* SocketBase::globalId = new std::atomic<uint32_t>();
    uint64_t SocketBase::TcpTime = 0;
    std::map<uint32_t, uint64_t>* SocketBase::globalRecvPortMap = new std::map<uint32_t, uint64_t>();
}
