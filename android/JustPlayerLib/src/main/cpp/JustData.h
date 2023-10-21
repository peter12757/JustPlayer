//
// Created by Peter Xi on 2022/5/14.
//

#ifndef ANDROIDTEST_JUSTDATA_H
#define ANDROIDTEST_JUSTDATA_H

#include "data/package.h"
#include "config/constants.h"

using namespace JustData;
using namespace JustData::Package;

enum JUST_Method {
    JUSTMETHOD_UNKNOW = 0,
    METHOD_PAUSE = JUSTMETHOD_UNKNOW+1,
    METHOD_PREPARE= METHOD_PAUSE+1,
    METHOD_START=METHOD_PREPARE+1,
    METHOD_STOP=METHOD_START+1,
    METHOD_RESET= METHOD_STOP+1,
    METHOD_SET_DATA_SOURCE=METHOD_RESET+1,
    METHOD_SEEK=METHOD_SET_DATA_SOURCE+1,
    METHOD_GET_DURATION=METHOD_SEEK+1,
    METHOD_GET_POSITION=METHOD_GET_DURATION+1,
};

enum JUST_URI {
    URI_UNKNOW =0,
    URI_METHOD=URI_UNKNOW+1,
};



#endif //ANDROIDTEST_JUSTDATA_H