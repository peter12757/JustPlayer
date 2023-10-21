//
// Created by Peter Xi on 2022/5/29.
//

#ifndef ANDROIDTEST_TIMERPOOL_H
#define ANDROIDTEST_TIMERPOOL_H


#pragma once
#include <set>
#include <stddef.h>
#include "../Util.h"
#include "../config/constants.h"

namespace XUtil
{
    class ITimerHandler;
    struct TimerNode
    {
    public:
        explicit TimerNode(uint32_t nextTimeoutStamp = 0,
                           uint32_t interval = 0,
                           ITimerHandler* ph = NULL);
        virtual ~TimerNode();

    public:
        bool operator<(const TimerNode& dest) const;

    public:
        uint32_t m_nextTimeoutStamp;
        uint32_t m_interval;
        ITimerHandler* m_pTimerHandler;
    };

    class TimerPool
    {
    public:
        virtual ~TimerPool();

    public:
        static void create();
        static void release();
        static TimerPool* instance();

    public:
        void onTimeout(uint32_t now);

    public:
        void addTimeout(uint32_t timeout, ITimerHandler* pTimerHandler);
        void deleteTimeout(ITimerHandler* pTimerHandler);

    private:
        TimerPool();

    private:
        static TimerPool* m_pInstance;

    private:
        std::set<TimerNode> m_timerSet;
    };
}// namespace liveengine



#endif //ANDROIDTEST_TIMERPOOL_H
