//
// Created by Peter Xi on 2022/5/29.
//

#include "Util.h"


#define USEC_PER_SEC 1000000ull
uint32_t Util::getTickCount()
{
#if defined(__ANDROID__)
    struct timespec now;
  clock_gettime(CLOCK_MONOTONIC, &now);
  return (uint32_t)((uint64_t)now.tv_sec * 1000 + (uint64_t)now.tv_nsec / USEC_PER_SEC);
#elif defined(_WIN32)
    return timeGetTime();
#elif defined(__APPLE__)
    if (__builtin_available(iOS 10.0, macOS 10.12, *))
  {
    struct timespec tsNowTime;
    clock_gettime(CLOCK_MONOTONIC, &tsNowTime);
    return (uint32_t)((uint64_t)tsNowTime.tv_sec * 1000 + (uint64_t)tsNowTime.tv_nsec / 1000000);
  }
  else
  {
    struct timeval now;
    gettimeofday(&now, NULL);
    return (uint32_t)(((uint64_t)now.tv_sec * 1000000 + now.tv_usec) / 1000);
  }
#else
#error Unspecified Platform for func(getTickCount)!
#endif
}

bool Util::isBiggerUint32(uint32_t src, uint32_t dest) { return (src != dest && src - dest < 0x7fffffff); }

bool Util::isEqualOrBiggerUint32(uint32_t src, uint32_t dest) { return (src - dest < 0x7fffffff); }
