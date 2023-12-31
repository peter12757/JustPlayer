//
// Created by PeterX on 2023/10/27 0027.
//

#ifndef JUSTPLAYER_TICKTIME_H
#define JUSTPLAYER_TICKTIME_H



#include <chrono>

namespace NET_SOCKET {
    class TickTime {
    public:
        static uint32_t MillisecondTimestamp();

        static uint64_t MillisecondTimestamp64();

        static uint64_t MicrosecondTimestamp64();

#ifdef TEST
        static void putNow(uint32_t iNow);
            static uint32_t mNow;
#endif
    };

#ifdef TEST

    inline void TickTime::putNow(uint32_t iNow)
        {
            mNow = iNow;
        }
        inline uint32_t TickTime::MillisecondTimestamp()
        {
            return mNow;
        }
#else

    inline uint64_t TickTime::MillisecondTimestamp64() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::steady_clock::now().time_since_epoch()).count();
    }

    inline uint32_t TickTime::MillisecondTimestamp() {
        return (uint32_t) MillisecondTimestamp64();
    }

    inline uint64_t TickTime::MicrosecondTimestamp64() {
        return std::chrono::duration_cast<std::chrono::microseconds>(
                std::chrono::steady_clock::now().time_since_epoch()).count();
    }

#endif
}



#endif //JUSTPLAYER_TICKTIME_H
