//
// Created by Peter Xi on 2022/5/21.
//

#ifndef ANDROIDTEST_SOCKETBASE_H
#define ANDROIDTEST_SOCKETBASE_H

//======================================================================================
#include "common.h"
#include "SocketHandler.h"
#include <atomic>
//#include "openssl/ssl.h"
#include "../base/data/package.h"
//======================================================================================
namespace NET_SOCKET {
    using namespace JustData::Package;


    class SocketBase
    {
    public:
        enum SocketType
        {
            TYPE_UDP = 0,
            TYPE_TCP,
        };

        SocketBase():m_iSocket(-1),m_pParam(NULL),m_pHandler(NULL), m_iTimeout(0), m_bEnanbe(true), m_selEvent(0)
                ,m_pingCount(0) ,m_checkCount(0), m_lastPingResTime(0), m_lastCheckResTime(0), m_groupId(-1), m_isIpv6(false)
        {
            m_socketId = globalId->fetch_add(1);
        }
        SocketBase(uint8_t type):m_iSocketType(type),m_iSocket(-1),m_pParam(NULL),m_pHandler(NULL), m_iTimeout(0), m_bEnanbe(true), m_selEvent(0)
                ,m_pingCount(0) ,m_checkCount(0), m_lastPingResTime(0), m_lastCheckResTime(0), m_groupId(-1)
        {
            m_socketId = globalId->fetch_add(1);
        }
        virtual ~SocketBase(){m_ip = -1; m_iPort = -1; m_iSocketType = -1; m_iSocket = -1; m_groupId = -1; m_pParam = NULL; m_pHandler = NULL;};

    public:
        virtual bool Connect(uint32_t uiIP, uint16_t iPort, bool async = false) = 0;
        //virtual bool Listen(uint32_t uiIP,short iPort,bool bTryNext) = 0;
        virtual void CloseTimeout(uint64_t tNow) = 0;
        virtual int onReadSocket() = 0;
        virtual int onWriteSocket() = 0;
        virtual void onError() = 0;
        virtual void CloseSocket() = 0;
        virtual int SendBin(uint32_t ip, uint16_t port, Marshallable& pPkg, uint32_t uri) = 0;
        virtual int SendBin(uint32_t ip, uint16_t port, const char* data, uint32_t len) = 0;
        virtual void setTimeout(uint32_t interval) = 0;
        virtual bool isProxyUsed() {return false;}
//        virtual SSL* GetSSL(){return nullptr;}

        int fd(){return m_iSocket;}
        int socketId() {return m_socketId;}
        int getPingCount(){return m_pingCount++;};
        int getCheckCount(){return m_checkCount++;};
        uint64_t lastPingResTime() {return m_lastPingResTime;};
        uint64_t lastCheckResTime() {return m_lastCheckResTime;};
        void setLastPingResTime(uint64_t time) {m_lastPingResTime = time;};
        void setLastCheckResTime(uint64_t time) {m_lastCheckResTime = time;};

    public:
        uint32_t m_ip;
        uint16_t m_iPort;
        uint8_t m_iSocketType;	// 0:udp,1:tcp,2:fifo,3:domain

        SOCKET m_iSocket;
        void *m_pParam;
        SocketHandler *m_pHandler;
        uint32_t m_iTimeout;
        bool m_bEnanbe;
        int m_selEvent;
        int m_groupId;
        bool m_isIpv6;

        //Workaround for resolving China Unicom network blocking while too many udp port are used in 3 minutes
#ifdef ANDROID
        static const int kMaxUdpCount = 25;
#else
        static const int kMaxUdpCount = 30;
#endif
        static std::map<uint32_t, uint64_t>* globalRecvPortMap;
#ifdef ANDROID
        static const int kPortExpireTime = 2 * 60 * 1000;
#else
        static const int kPortExpireTime = 60 * 1000;
#endif
        static uint64_t TcpTime;

    private:
        int m_pingCount;
        int m_checkCount;
        uint64_t m_lastPingResTime;
        uint64_t m_lastCheckResTime;
        //liuli 2014-03-07 add a unique inc id instead of the address of this object instance
        uint32_t m_socketId;
        static std::atomic<uint32_t>* globalId;
        static std::map<uint32_t, uint64_t> createPortMap();

    };
}


#endif //ANDROIDTEST_SOCKETBASE_H
