//
// Created by Peter Xi on 2022/5/29.
//

#include "ITimerHandler.h"
#include "TimerPool.h"
#include "../util/Util.h"

using namespace XUtil;

TimerPool* TimerPool::m_pInstance = NULL;

TimerNode::TimerNode(uint32_t nextTimeoutStamp, uint32_t interval, ITimerHandler* ph)
        : m_nextTimeoutStamp(nextTimeoutStamp)
        , m_interval(interval)
        , m_pTimerHandler(ph)
{
}

TimerNode::~TimerNode() {}

bool TimerNode::operator<(const TimerNode& dest) const
{
    if (Util::isBiggerUint32(dest.m_nextTimeoutStamp, m_nextTimeoutStamp))
    {
        return true;
    }

    if (m_nextTimeoutStamp == dest.m_nextTimeoutStamp && m_pTimerHandler < dest.m_pTimerHandler)
    {
        return true;
    }

    return false;
}

TimerPool::TimerPool() {}

TimerPool::~TimerPool() {}

void TimerPool::create()
{
    if (!m_pInstance)
    {
        m_pInstance = new TimerPool();
    }
}

TimerPool* TimerPool::instance() { return m_pInstance; }

void TimerPool::release()
{
    if (!m_pInstance)
    {
        return;
    }

    SafeDelete(m_pInstance);
}

void TimerPool::addTimeout(uint32_t timeout, ITimerHandler* pTimerHandler)
{
    uint32_t now = Util::getTickCount();
    TimerNode newNode(now + timeout, timeout, pTimerHandler);
    m_timerSet.insert(newNode);
}

void TimerPool::deleteTimeout(ITimerHandler* pTimerHandler)
{
    for (std::set<TimerNode>::iterator it = m_timerSet.begin(); it != m_timerSet.end();)
    {
        if (it->m_pTimerHandler == pTimerHandler)
        {
            m_timerSet.erase(it++);
        }
        else
        {
            ++it;
        }
    }
}

void TimerPool::onTimeout(uint32_t now)
{
    const uint32_t kInvalidTimeout = 30 * 1000;
    std::set<TimerNode>::iterator it = m_timerSet.begin();
    while (it != m_timerSet.end() && Util::isEqualOrBiggerUint32(now, it->m_nextTimeoutStamp))
    {
        uint32_t nextTimeoutStamp = it->m_nextTimeoutStamp + it->m_interval;
        if (now - it->m_nextTimeoutStamp > kInvalidTimeout)
        {
            nextTimeoutStamp = now;
        }

        uint32_t timeoutInterval = it->m_interval;
        ITimerHandler* pTimerHandler = it->m_pTimerHandler;
        m_timerSet.erase(it);
        m_timerSet.insert(TimerNode(nextTimeoutStamp, timeoutInterval, pTimerHandler));
        // onTimeout函数内部，有可能调用addTimeout以及deleteTimeout，导致m_timerSet数据改变
        pTimerHandler->onTimeout();
        it = m_timerSet.begin();
    }
}
