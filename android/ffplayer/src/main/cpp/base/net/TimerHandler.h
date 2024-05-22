//
// Created by Peter Xi on 2022/5/20.
//

#ifndef ANDROIDTEST_TIMERHANDLER_H
#define ANDROIDTEST_TIMERHANDLER_H

#include "../log/LogUtil.h"
#include <atomic>
#include <math.h>
#include "TickTime.h"
namespace NET_SOCKET {
    class TimerHandler {
    public:
        TimerHandler() : mLastSec(0), mIntervSec(1) {
            m_handlerId = globalId->fetch_add(1);
        };

        virtual ~TimerHandler() {};

        void InitTimer(int intervSec) {
            if (intervSec >= 1) {
                mLastSec = ceil(TickTime::MillisecondTimestamp64() * 1.0 / 1000);
            } else {
                mLastSec = TickTime::MillisecondTimestamp64() / 1000;
            }

            mIntervSec = intervSec;
        };

        virtual bool Timer() = 0; //just return false if need to remove from the looper.

        bool TimerCheck(uint64_t nowSec) {
            bool valid = true;
            if ((int64_t) (nowSec - mLastSec) >= mIntervSec) {
                valid = Timer();
                mLastSec = nowSec;
            }
            return valid;
        };

        uint32_t handlerId() const { return m_handlerId; }

    protected:
        int64_t mIntervSec; //timer interval in seconds
        uint64_t mLastSec; //last check seconds
    private:
        // add a unique id to identify instead of using the address
        uint32_t m_handlerId;
        static std::atomic<uint32_t> *globalId;
    };
}
#endif //ANDROIDTEST_TIMERHANDLER_H
