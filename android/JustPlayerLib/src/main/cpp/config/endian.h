//
// Created by Peter Xi on 2022/5/30.
//

#ifndef ANDROIDTEST_ENDIAN_H
#define ANDROIDTEST_ENDIAN_H

#define LIL_ENDIAN  1234
#define BIG_ENDIAN  4321

#ifndef BYTEORDER
#ifdef __linux__
#include <endian.h>
#define SDL_BYTEORDER  __BYTE_ORDER
#else /* __linux __ */
#if defined(__hppa__) || \
    defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
    (defined(__MIPS__) && defined(__MISPEB__)) || \
    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || \
    defined(__sparc__)
#define BYTEORDER   SDL_BIG_ENDIAN
#else
#define BYTEORDER   LIL_ENDIAN
#endif
#endif
#endif

#endif //ANDROIDTEST_ENDIAN_H
