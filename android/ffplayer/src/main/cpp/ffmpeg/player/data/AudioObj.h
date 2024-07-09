//
// Created by Peter Xi on 2022/6/3.
//

#ifndef ANDROIDTEST_AUDIOOBJ_H
#define ANDROIDTEST_AUDIOOBJ_H
#include "FFObj.h"
#include <sstream>


#define SAMPLE_QUEUE_SIZE 9

/* we use about AUDIO_DIFF_AVG_NB A-V differences to make the average */
#define AUDIO_DIFF_AVG_NB   20


typedef struct AudioParams {
    int freq;
    int channels;
    int64_t channel_layout;
    enum AVSampleFormat fmt;
    int frame_size;
    int bytes_per_sec;
} AudioParams;

class AudioObj {
public:
    AudioObj();
    virtual ~AudioObj();


public:
    FrameQueue *sampq;                       // 音频队列
    Clock *audclk;                               // 音频时钟

    int audio_stream;                           // 音频码流Id

    double audio_clock;                     // 音频时钟
    int audio_clock_serial;                 // 音频时钟序列
    double audio_diff_cum;                  // 用于音频差分计算 /* used for AV difference average computation */
    double audio_diff_avg_coef;         //音频时钟与同步源时差均值加权系数
    double audio_diff_threshold;            // 音频差分阈值
    int audio_diff_avg_count;               // 平均差分数量
    AVStream *audio_st;                     // 音频码流
    PacketQueue *audioq;                 // 音频包队列
    int audio_hw_buf_size;                  // 硬件缓冲大小
    uint8_t *audio_buf;                     // 音频缓冲区
    uint8_t *audio_buf1;                        // 音频缓冲区1
    unsigned int audio_buf_size;            // 音频缓冲大小 /* in bytes */
    unsigned int audio_buf1_size;       // 音频缓冲大小1
    int audio_buf_index;                        // 音频缓冲索引 /* in bytes */
    int audio_write_buf_size;               // 音频写入缓冲大小
    int audio_volume;                           // 音量
    int muted;                                      // 是否静音
    struct AudioParams audio_src;       // 音频参数
    struct AudioParams audio_filter_src; // 音频过滤器
    struct AudioParams audio_tgt;       // 音频参数
    struct SwrContext *swr_ctx;         // 音频转码上下文
    int frame_drops_early;                  //
    int frame_drops_late;                       //


    AVFilterContext *in_audio_filter;   // 第一个音频过滤器 // the first filter in the audio chain
    AVFilterContext *out_audio_filter;  // 最后一个音频过滤器 // the last filter in the audio chain



};


#endif //ANDROIDTEST_AUDIOOBJ_H
