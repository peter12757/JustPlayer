//
// Created by Peter Xi on 2022/5/20.
//

#include "Sellector_Epoll.h"
#include "TickTime.h"

using namespace NET_SOCKET;


namespace NET_SOCKET{
    static int pipe_err = 0;
    uint64_t Sellector_Epoll::m_iHaoMiao = 0;
    uint64_t Sellector_Epoll::m_iNow     = 0;

    static void epoll_pipe_handler(int){
        pipe_err++;
    }


    Sellector_Epoll::Sellector_Epoll()
            : mTimerPreparer(NULL)
            , mMaxFd(0)
    {
        m_iHaoMiao = TickTime::MillisecondTimestamp64();
        m_iNow = m_iHaoMiao/1000;
        interval = 10; // default 10 ms
        mTimerCheckPending = false;
    }

    //======================================================================================
    Sellector_Epoll::~Sellector_Epoll()
    {
        if (m_bRuning == true)
            m_bRuning = false;
    }
    //======================================================================================
    bool Sellector_Epoll::Init()
    {
#ifndef _WIN32
        if(signal(SIGPIPE, epoll_pipe_handler) == SIG_ERR) {
            LOGE("signal return error, SIGPIPE");
        }
#endif

        m_bRuning = true;
        return true;
    }
    bool Sellector_Epoll::running() const
    {
        return m_bRuning;
    }
    //======================================================================================
    void Sellector_Epoll::Stop()
    {
        LOGI("stop selector %p", this);
        m_bRuning = false;
    }
    //======================================================================================
    void Sellector_Epoll::SetInterval(int interval) {
        this->interval = interval;
    }
    //======================================================================================

    void Sellector_Epoll::setWREFdSet(fd_set* rdset, fd_set* wrset, fd_set* exceptset)
    {
        //  logCat(Info, "Sellector_Epoll:begin setWREFdSet");

        std::lock_guard<std::recursive_mutex> guard(mSockSetLock);

        FD_ZERO(rdset);
        FD_ZERO(wrset);
        FD_ZERO(exceptset);
        mMaxFd = 0;
        for(std::map<uint32_t, SocketBase*>::iterator it=m_setSocket.begin();it!=m_setSocket.end(); it ++)
        {
            uint32_t socketId = it->first;
            SocketBase* s =  it->second;

            if (isRemoved(socketId) || s == NULL || (s->fd() == -1) || (s->fd() >= FD_SETSIZE))
            {
                //LOGD("setWREFdSet isRemoved(s) %d s %u", isRemoved(socketId), (uint32_t)s);
                continue;
            }

            if (mMaxFd < s->fd()) {
                mMaxFd = s->fd();
            }

            if (SEL_READ & s->m_selEvent) {
                //logCat(Info, "Sellector_Epoll: setWREFdSet read fd:%u",s->fd());
                FD_SET(s->fd(), rdset);
                FD_SET(s->fd(), exceptset);
            }
            if (SEL_WRITE & s->m_selEvent) {
                //logCat(Info, "Sellector_Epoll: setWREFdSet write fd:%u",s->fd());
                FD_SET(s->fd(), wrset);
                FD_SET(s->fd(), exceptset);
            }
            //LOGD("setWREFdSet fd %u event %u maxfd %u sockaddr:%u socketId %u", s->fd(), s->m_selEvent, mMaxFd, (uint32_t)s, s->socketId());

        }
    }

    void Sellector_Epoll::Run()
    {
        LOGD("Sellector_Epoll Enter Run");
        std::unique_ptr<fd_set> rdset(new fd_set());
        std::unique_ptr<fd_set> wrset(new fd_set());
        std::unique_ptr<fd_set> exceptset(new fd_set());
        m_iHaoMiao = TickTime::MillisecondTimestamp64();
        m_iNow = m_iHaoMiao/1000;
        lastIterval = 0;
        lastRedress = lastIntervalTimeOut = m_iHaoMiao;
        while (m_bRuning)
        {
            if (checkHopAndGetTime())
            {
                CloseTimeout(m_iNow);
                TimerCheck(m_iNow);
            }else if(mTimerCheckPending){
                LOGD("selector:timer check immediately.");
                TimerCheck(m_iNow);
            }
            setWREFdSet(rdset.get(), wrset.get(), exceptset.get());

            int exceptCount = 0;

            struct timeval timep;
            timep.tv_sec = 0;
            timep.tv_usec = interval * 1000;
            //liuli 2014-01-23 fix this, use mMaxFd+1 insteadof 100
            int iActive = select(mMaxFd + 1, rdset.get(), wrset.get(), exceptset.get(), &timep);

            if (iActive < 0 )
            {
#ifdef _WIN32
                Sleep(1);
#else
                usleep(1000);
#endif
                if (EINTR == errno){
                    continue;
                }
            }

            if(iActive > 0){
                //LOGD("epoll iterating begins.");
                std::lock_guard<std::recursive_mutex> guard(mSockSetLock);

                for(std::map<uint32_t, SocketBase*>::iterator it= m_setSocket.begin();it!= m_setSocket.end(); it ++)
                {
                    uint32_t socketId = it->first;
                    SocketBase* s =  it->second;

                    if (isRemoved(socketId) || s == NULL || (s->fd() == -1) || (s->fd() >= FD_SETSIZE))
                    {
                        continue;
                    }

                    if (FD_ISSET(s->fd(), exceptset.get())) {
                        if (isRemoved(socketId)){
                            LOGD("Error happened on deleted socket");
                            continue;
                        }

                        ++exceptCount;

                        s->onError();
                        continue;
                    }
                    if (FD_ISSET(s->fd(), rdset.get())) {
                        //LOGD("happened FD_ISSET rdset fd:%u", s->fd());
                        onReadSocket(socketId, s);
                    }
                    if (isRemoved(socketId) || (s->fd() == -1))
                    {
                        LOGD("Maybe socketBase has deleted by onReadSocket,so check it again");
                        continue;
                    }
                    if (FD_ISSET(s->fd(), wrset.get())) {
                        //LOGD("happened FD_ISSET wrset fd:%u", s->fd());
                        onWriteSocket(socketId, s);
                    }
                }
                //LOGD("epoll iterating end.");
            }
            clearRemoved();

#ifdef _WIN32
            // 如果网络不可用则sleep
			if(iActive == -1 || exceptCount == iActive) {
				Sleep(1);
			}
#endif

/*            if(iActive == mMaxFd)
            {
#ifdef IOS
                LOGE("epoll reach the max size:%d m_setSocket:%lu", 100, m_setSocket.size());
#else
                LOGE("epoll reach the max size:%d m_setSocket:%u", 100, m_setSocket.size());
#endif
            }*/

        }
        LOGI("exit the main loop");
    }
    //======================================================================================
    void Sellector_Epoll::CloseTimeout(uint64_t tNow)
    {
        std::lock_guard<std::recursive_mutex> guard(mSockSetLock);

        for(std::map<uint32_t, SocketBase*>::iterator it=m_setSocket.begin();it!=m_setSocket.end(); it ++)
        {
            uint32_t socketId = it->first;
            SocketBase* s =  it->second;

            if(isRemoved(socketId)) {
                continue;
            }
            s->CloseTimeout(tNow);
        }
    }
    //======================================================================================
    void Sellector_Epoll::TimerCheck(uint64_t tNow)
    {
        if(mTimerPreparer != NULL){
            mTimerPreparer->onPrepare();
        }

        std::lock_guard<std::recursive_mutex> guard(mTimerSetLock);

        //updating current timer set first.
        // first add then remove, as a TimerHandler may be added and removed during one interval.
        for(std::set<TimerHandler*>::iterator it= m_setTimerToAdd.begin(); it != m_setTimerToAdd.end();
            ++it)
        {
            m_setTimer[(*it)->handlerId()] = *it;
        }
        m_setTimerToAdd.clear();

        for(std::set<uint32_t>::iterator it= m_setTimerToRemove.begin(); it != m_setTimerToRemove.end();
            ++it)
        {
            m_setTimer.erase(*it);
        }
        m_setTimerToRemove.clear();

        //iterating timer set safely.
        for(std::map<uint32_t, TimerHandler*>::iterator it=m_setTimer.begin();it!=m_setTimer.end();)
        {
            if(m_setTimerToRemove.find(it->first) != m_setTimerToRemove.end())
            {
                it++;
                continue;
            }
            if(!(it->second)->TimerCheck(tNow)){
                //client request to remove.
                m_setTimer.erase(it++);
            }else{
                //iterate to next.
                ++it;
            }
        }
        mTimerCheckPending = false;
    }

    //======================================================================================
    // timer check interval 10ms
    void Sellector_Epoll::TimerCheck2(uint64_t ms)
    {
        std::lock_guard<std::recursive_mutex> guard(mTimer2SetLock);
        for(std::set<TimerHandler2*>::iterator it=m_setTimer2.begin();it!=m_setTimer2.end();)
        {
            TimerHandler2* timer2 = *it++;
            timer2->TimerCheck2(ms);
        }
    }

    //======================================================================================
    void Sellector_Epoll::AddSocket(SocketBase* s)
    {
        if(s == NULL) {
            return;
        }

        if (s->fd() >= FD_SETSIZE) {
            LOGE("socket fd(%d) >= FD_SETSIZE(%d)", s->fd(), FD_SETSIZE);
        }

        std::lock_guard<std::recursive_mutex> guard(mSockSetLock);
        if(m_setSocket.find(s->socketId()) == m_setSocket.end())
        {
            m_setSocket[s->socketId()] = s;
        }
    }

    //======================================================================================
    void Sellector_Epoll::SetEvent(SocketBase* s, int remove, int add)
    {
        //LOGE("Sellector_Epoll SetEvent socket<0x%08x> remove:%u add:%u", (uint32_t)s, remove, add);
        if(s == NULL || s->fd() == -1) {
            return;
        }
        //fix deadlock
        /*{
            std::lock_guard<std::recursive_mutex> guard(mSockSetLock);
            if(m_setSocket.find(s->socketId()) == m_setSocket.end())
            {
                m_setSocket[s->socketId()] = s;
            }
        }*/

        if ((SEL_READ & remove)
            || (SEL_WRITE & remove)) {
            s->m_selEvent = (s->m_selEvent) & (~remove);
        }
        if ((SEL_READ & add)
            || (SEL_WRITE & add)) {
            s->m_selEvent = (s->m_selEvent | add);
        }

        LOGD("Sellector_Epoll: SetEvent fd:%u sockaddr:%p socketId:%u",s->fd(), s, s->socketId());
    }
    //======================================================================================
    int Sellector_Epoll::onReadSocket(uint32_t socketId, SocketBase *pSocket)
    {
        if(pSocket == NULL/* || pSocket->m_iSocket == -1*/)
            return 0;

        if (isRemoved(socketId))
        {
            LOGD("DESTROY IN LOOP FOUND");
            return 0;
        }

        int ret = 0;
        try
        {
            ret = pSocket->onReadSocket();
        }
        catch (std::exception & ex)
        {
            LOGE("Sox Exception:%s, %u", ex.what(), pSocket->m_iSocket);
        }

        return ret;
    }
    //======================================================================================
    int Sellector_Epoll::onWriteSocket(uint32_t socketId, SocketBase *pSocket)
    {
        if(pSocket == NULL/* || pSocket->m_iSocket == -1 || pSocket->m_iSocket == 0*/)
        {
            //LOGD("onWriteSocket socket has close, sockaddr:%u", pSocket->m_iSocket);
            return 0;
        }

        if (isRemoved(socketId))
        {
            LOGD("onWriteSocket DESTROY IN LOOP FOUND " );
            return 0;
        }

        int ret = 0;
        //try
        // {
        ret = pSocket->onWriteSocket();
        // }
        //catch (std::exception & ex)
        // {
        //   showLog(ERROR_LEVEL, "Sox Exception:%s, %u", ex.what(), pSocket->m_iSocket);
        // }

        return ret;
    }
    //======================================================================================
    void Sellector_Epoll::removeSocket(SocketBase* s)
    {
        //LOGD("removeSocket,sockid:%u sockaddr:%u socketId %u ", s->fd(), (uint32_t)s, s->socketId());
        record_removed(s->socketId());
    }

    //======================================================================================
    void Sellector_Epoll::AddTimerHandler(TimerHandler *pHandler)
    {
        std::lock_guard<std::recursive_mutex> guard(mTimerSetLock);

        if(m_setTimerToAdd.find(pHandler)!=m_setTimerToAdd.end())
            return;

        m_setTimerToAdd.insert(pHandler);

        if(m_setTimerToRemove.find(pHandler->handlerId()) != m_setTimerToRemove.end())
            m_setTimerToRemove.erase(pHandler->handlerId());

        mTimerCheckPending = true;
    }

    void Sellector_Epoll::RemoveTimerHandler(TimerHandler *pHandler)
    {
        std::lock_guard<std::recursive_mutex> guard(mTimerSetLock);
        std::set<uint32_t>::iterator it = m_setTimerToRemove.find(pHandler->handlerId());
        if(it != m_setTimerToRemove.end())
            return;

        m_setTimerToRemove.insert(pHandler->handlerId());
        if(m_setTimerToAdd.find(pHandler) != m_setTimerToAdd.end())
            m_setTimerToAdd.erase(pHandler);
    }

    //======================================================================================
    void Sellector_Epoll::AddTimerHandler2(TimerHandler2 *pHandler)
    {
        std::lock_guard<std::recursive_mutex> guard(mTimer2SetLock);
        if(m_setTimer2.find(pHandler)!=m_setTimer2.end())
            return;
        pHandler->m_tLast = m_iHaoMiao;
        m_setTimer2.insert(pHandler);
    }

    //======================================================================================
    void Sellector_Epoll::RemoveTimerHandler2(TimerHandler2 *pHandler)
    {
        std::lock_guard<std::recursive_mutex> guard(mTimer2SetLock);
        std::set<TimerHandler2*>::iterator it = m_setTimer2.find(pHandler);
        if(it != m_setTimer2.end())
            m_setTimer2.erase(it);
    }

    bool Sellector_Epoll::checkHopAndGetTime()
    {
        uint64_t nowTimestamp = TickTime::MillisecondTimestamp64();
        uint32_t interval_msec = (uint32_t)(nowTimestamp - lastRedress);
        uint32_t diffInterval = (uint32_t)(nowTimestamp - lastIntervalTimeOut);
        m_iHaoMiao = nowTimestamp;
        m_iNow = m_iHaoMiao/1000;
        if (diffInterval > interval)
        {
            TimerCheck2(m_iHaoMiao);
            lastIntervalTimeOut = nowTimestamp;
        }
        if (interval_msec > 1000)
        {
            lastRedress = nowTimestamp;
            return true;
        }
        return false;
    }

    void Sellector_Epoll::clearRemoved()
    {
        std::lock_guard<std::recursive_mutex> guard(mSockSetLock);
        for(std::set<uint32_t>::iterator it = m_removed.begin(); it != m_removed.end(); it++) {
            m_setSocket.erase(*it);
        }
        m_removed.clear();
    }

    void Sellector_Epoll::record_removed(uint32_t socketId)
    {
        std::lock_guard<std::recursive_mutex> guard(mSockSetLock);
        m_removed.insert(socketId);
    }


}


