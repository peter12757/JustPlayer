//
// Created by Peter Xi on 2022/5/20.
//

#ifndef ANDROIDTEST_SELLECTOR_EPOLL_H
#define ANDROIDTEST_SELLECTOR_EPOLL_H

#include "TimerHandler.h"
#include "TimeHandler2.h"
#include <signal.h>
#include <mutex>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "SocketBase.h"

#define EPOLL_TIMEOUT 1

namespace NET_SOCKET{
    enum {
        SEL_NONE = 0, SEL_ALL = -1, SEL_READ = 1, SEL_WRITE = 2, SEL_RW = 3, // SEL_ERROR = 4,

        // notify only
        SEL_TIMEOUT = 8,

        // setup only, never notify
        // SEL_R_ONESHOT = 32, SEL_W_ONESHOT = 64, SEL_RW_ONESHOT = 96,
        SEL_CONNECTING = 128
    };


    class TimerHandler2;

    class IPrepareTimerCheck
    {
    public:
        virtual void onPrepare() = 0;
        virtual ~IPrepareTimerCheck() {}
    };


    class Sellector_Epoll
    {
    public:
        Sellector_Epoll();
        ~Sellector_Epoll();
        bool Init();
        void Stop();
        void Run();

        void setTimerPrepareHandler(IPrepareTimerCheck *handler) { mTimerPreparer = handler; }

        void CloseTimeout(uint64_t tNow);
        void TimerCheck(uint64_t tNow);
        void TimerCheck2(uint64_t ms);

        void SetEvent(SocketBase* s, int remove, int add);
        void SetEvent(SocketBase * s,bool bRead,bool bWrite,bool bDel);
        void AddSocket(SocketBase* s);
        int onReadSocket(uint32_t socketId, SocketBase *pSocket);
        int onWriteSocket(uint32_t socketId, SocketBase *pSocket);

        void AddTimerHandler(TimerHandler *pHandler); //thread safe
        void RemoveTimerHandler(TimerHandler *pHandler); //thread safe

        void AddTimerHandler2(TimerHandler2 *pHandler);
        void RemoveTimerHandler2(TimerHandler2 *pHandler);

        void removeSocket(SocketBase* s);
        void SetInterval(int interval);

        bool running() const;
    public:
        // check : destroy in loop
        bool isRemoved(uint32_t socketId) const	{ return m_removed.find(socketId) != m_removed.end(); }
        void clearRemoved();
        void record_removed(uint32_t socketId);
        bool checkHopAndGetTime();

    private:
        void setWREFdSet(fd_set* rdset, fd_set* wrset, fd_set* exceptset);

    public:
        static uint64_t m_iHaoMiao;
        static uint64_t m_iNow;

    protected:
        bool mTimerCheckPending; //set this flag when a timer task added.
        volatile bool m_bRuning;
        std::map<uint32_t, SocketBase *> m_setSocket;
        std::recursive_mutex mSockSetLock;

        std::map<uint32_t, TimerHandler*> m_setTimer;
        std::recursive_mutex mTimerSetLock;

        //modifications to timer set first goes here, then updates to m_setTimer before TimerCheck().
        std::set<TimerHandler*> m_setTimerToAdd;
        std::set<uint32_t> m_setTimerToRemove;
        IPrepareTimerCheck *mTimerPreparer;

        std::set<uint32_t> m_removed;
        std::set<TimerHandler2*> m_setTimer2;
        std::recursive_mutex mTimer2SetLock;
        int interval;

        SOCKET mMaxFd; //linux use this in select
    private:
        uint64_t lastRedress;
        uint32_t lastIterval;
        uint64_t lastIntervalTimeOut;
    };
}
#endif //ANDROIDTEST_SELLECTOR_EPOLL_H
