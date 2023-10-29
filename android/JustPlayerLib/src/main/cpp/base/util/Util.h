//
// Created by Peter Xi on 2022/5/29.
//

#ifndef ANDROIDTEST_UTIL_H
#define ANDROIDTEST_UTIL_H
#include "../log/LogUtil.h"
#include "../const/just_base_constants1.h"


#ifndef MAX
#define MAX(a, b)    ((a) > (b) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b)    ((a) < (b) ? (a) : (b))
#endif


class Util {
public:
    static uint32_t getTickCount();
    static bool isBiggerUint32(uint32_t src, uint32_t dest);
    static bool isEqualOrBiggerUint32(uint32_t src, uint32_t dest);


};


#endif //ANDROIDTEST_UTIL_H
