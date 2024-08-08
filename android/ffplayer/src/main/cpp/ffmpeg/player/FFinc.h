//
// Created by Peter Xi on 2022/6/3.
//

#ifndef ANDROIDTEST_FFINC_H
#define ANDROIDTEST_FFINC_H
extern "C"{
#include "../ffmpeginc/libavutil/avutil.h"
#include "../ffmpeginc/libavcodec/avcodec.h"
#include "../ffmpeginc/libavformat/avformat.h"
#include "../ffmpeginc/libavdevice/avdevice.h"
#include "../ffmpeginc/libavfilter/avfilter.h"
#include "../ffmpeginc/libavutil/log.h"
#include "../ffmpeginc/libavcodec/avfft.h"
#include "../ffmpeginc/libavutil/fftime.h"
#include "../ffmpeginc/libavutil/dict.h"
#include "../ffmpeginc/libavutil/channel_layout.h"
#include "../ffmpeginc/libavutil/samplefmt.h"
#include "../ffmpeginc/libavfilter/buffersrc.h"
#include "../ffmpeginc/libavfilter/buffersink.h"
#include "../ffmpeginc/libavutil/fifo.h"
#include "../ffmpeginc/libswscale/swscale.h"
#include "../ffmpeginc/libavutil/error.h"

#include "../ffmpeginc/libavutil/file.h"
#include "../ffmpeginc/libavutil/mem.h"
#include "../ffmpeginc/libavutil/avstring.h"
#include "../ffmpeginc/libavutil/pixdesc.h"
#include "../ffmpeginc/libswresample/swresample.h"
}
#include <sstream>

//#include "config/FFConfig.h"
//#include "constants.h"
//#include "common.h"
//#include "PacketQueue.h"
//#include "FrameQueue.h"
//#include "MyAVPacketList.h"
//#include "AudioParams.h"


#define MAX_QUEUE_SIZE (15 * 1024 * 1024)
#define MIN_FRAMES 25



enum AVSyncType{
    AV_SYNC_AUDIO_MASTER, /* default choice */
    AV_SYNC_VIDEO_MASTER,
    AV_SYNC_EXTERNAL_CLOCK, /* synchronize to an external clock */
};



#endif //ANDROIDTEST_FFINC_H
