//
// Created by Peter Xi on 2022/5/21.
//

#ifndef ANDROIDTEST_COMMON_H
#define ANDROIDTEST_COMMON_H


#ifdef _WIN32
#include <Windows.h>
#ifndef socklen_t
#define socklen_t int
#endif
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#ifndef SOCKET
#define SOCKET  int
#endif
#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#endif


#include <stdint.h>
#include <string.h>
#include <stdio.h>
#if defined(_LIBCPP_VERSION)
#include <memory>
#else
#include <tr1/memory>
#endif

#if defined(_LIBCPP_VERSION)
#define STD_NS std
#else
#define STD_NS std::tr1
#endif

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName&) = delete;    \
    void operator=(const TypeName&) = delete;

//safe release object ptr & ptr array
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

//define
static const int TCP_IP_HEADER_LENGTH = 20+20;
static const int UDP_IP_HEADER_LENGTH = 8+20;

#include "../base/data/package.h"

#endif //ANDROIDTEST_COMMON_H
