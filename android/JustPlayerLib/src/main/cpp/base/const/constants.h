//
// Created by Peter Xi on 2022/5/13.
//

#ifndef ANDROIDTEST_CONSTANT_H
#define ANDROIDTEST_CONSTANT_H

#include <string>
#include <iostream>
using namespace std;


enum RET{
    RET_OK = 0,
    RET_ERR = 1000,
    RET_WARNING = 10000,
};

#define NOT_IMPL LOGE("NOT IMPL!!!")

#ifndef NELEM
#define NELEM(x) ((int) (sizeof(x) / sizeof((x)[0])))
#endif


#define VIDEO_PICTURE_QUEUE_SIZE 3
#define SUBPICTURE_QUEUE_SIZE 16
#define SAMPLE_QUEUE_SIZE 9
#define FRAME_QUEUE_SIZE FFMAX(SAMPLE_QUEUE_SIZE, FFMAX(VIDEO_PICTURE_QUEUE_SIZE, SUBPICTURE_QUEUE_SIZE))

template< class T >
inline void SafeDelete(T* & pVal)
{
    delete pVal;
    pVal = 0;
}

template< class T >
inline void SafeDeleteArray(T* & pVal)
{
    delete[] pVal;
    pVal = 0;
}

//for XThread.h
#define PAL_EVENT_10_SEC   10000
#define PAL_EVENT_INFINITE 0xffffffff
#endif //ANDROIDTEST_CONSTANT_H
