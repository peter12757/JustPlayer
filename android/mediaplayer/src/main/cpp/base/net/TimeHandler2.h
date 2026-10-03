//
// Created by Peter Xi on 2022/5/20.
//

#ifndef ANDROIDTEST_TIMEHANDLER2_H
#define ANDROIDTEST_TIMEHANDLER2_H

// to do
// timer check every 50ms
#include "Sellector_Epoll.h"

namespace NET_SOCKET {

    class TimerHandler2 {
    public:
        TimerHandler2() : m_tLast(0) {};

        virtual ~TimerHandler2() {}

        void InitTimer2(int iInterval) {
//            m_tLast = Sellector_Epoll::m_iHaoMiao;
        m_tLast = 0;
            m_iInterval = iInterval;
        };

        virtual void Timer2() = 0;

        void TimerCheck2(uint64_t tNow) {
            uint32_t diff = (uint32_t) (tNow - m_tLast);
            if (diff >= m_iInterval &&
                diff < 0xEFFFFFFF) // in case tNow is a little smaller than m_tLast;
            {
                Timer2();
                m_tLast = tNow;
            }
        };
    protected:
        int m_iInterval;

    public:
        uint64_t m_tLast;
    };
}

#endif //ANDROIDTEST_TIMEHANDLER2_H
