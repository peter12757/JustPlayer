//
// Created by Peter Xi on 2022/5/14.
//

#ifndef ANDROIDTEST_JUSTDATA_H
#define ANDROIDTEST_JUSTDATA_H

#include "../protocol/package.h"
#include "../log/LogUtil.h"


//#define JUST_UNMARSHAL(data, up)                                                        \
//  do                                                                                    \
//  {                                                                                     \
//    data.unmarshal(up);                                                                  \
//    if (up.isUnpackError())                                                             \
//    {                                                                                   \
//      LOGE(LOG_TAG, "unmarshal error in func %s, type %u", __FUNCTION__, data.event); \
//      return 0;                                                                         \
//    }                                                                                   \
//  } while (0)

#define JUST_UNMARSHAL(data, up)                                                        \
  do                                                                                    \
  {                                                                                     \
    data.unmarshal(up);                                                                  \
  } while (0)

using namespace JustPackage;

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

enum JustPlayerType {
    MediaCodec = 0,
    FFMPEGPlayer = MediaCodec+1,
};

struct JustContext : public IPackage{
    JustPlayerType playerType = MediaCodec;

    JustContext()
    {}

    void marshal(Pack &p) const override
    {
        p << (::uint32_t) playerType;
    }

    void unmarshal(const Unpack &up) override
    {
        ::uint32_t type = 0;
        up>>type;
        playerType = static_cast<JustPlayerType>(type);
    }

    string PlayerType2Str()
    {
        switch (playerType) {
            case MediaCodec: return "MediaCodec";
            case FFMPEGPlayer: return "FFMPEGPlayer";
            default: return "Unknown";
        }
    }


};



#endif //ANDROIDTEST_JUSTDATA_H
