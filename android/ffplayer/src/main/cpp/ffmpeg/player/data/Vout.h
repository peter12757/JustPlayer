//
// Created by Peter Xi on 2022/6/3.
//

#ifndef ANDROIDTEST_VIDEOOBJ_H
#define ANDROIDTEST_VIDEOOBJ_H
#include "FFinc.h"
#include "FrameQueue.h"


#define VIDEO_PICTURE_QUEUE_SIZE 3
//
//class Vout {
//public:
//    Vout();
//    virtual ~Vout();
//
//public:
//    FrameQueue *pictq;   // 视频队列
//    Clock *vidclk;                              // 视频时钟
//
//    double frame_timer;                     // 帧计时器
//    double frame_last_returned_time;    // 上一次返回时间
//    double frame_last_filter_delay;     // 上一个过滤器延时
//    int video_stream;                           // 视频码流Id
//    AVStream *video_st;                     // 视频码流
//    PacketQueue *videoq;                 // 视频包队列
//    double max_frame_duration;          // 最大帧显示时间 // maximum duration of a frame - above this, we consider the jump a timestamp discontinuity
//    struct SwsContext *img_convert_ctx; // 视频转码上下文
//    struct SwsContext *sub_convert_ctx; // 字幕转码上下文
//     int width, height, xleft, ytop;         // 宽高，其实坐标
//    int step;                                       // 步进
//
//
//    AVFilterContext *in_video_filter;   // 第一个视频滤镜 // the first filter in the video chain
//    AVFilterContext *out_video_filter;  // 最后一个视频滤镜 // the last filter in the video chain
//
//};


#endif //ANDROIDTEST_VIDEOOBJ_H
