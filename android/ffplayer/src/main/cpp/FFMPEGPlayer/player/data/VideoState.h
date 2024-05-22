//
// Created by Peter Xi on 2022/5/25.
//

#ifndef ANDROIDTEST_VIDEOSTATE_H
#define ANDROIDTEST_VIDEOSTATE_H

#include <string>
#include "FFinc.h"
#include "VideoObj.h"
#include "AudioObj.h"
#include "SubtitleObj.h"
#include "../../../base/config/constants.h"
#include "audio/AudioBaseEngine.h"
#include <sstream>
#include "VideoState.h"
#include "../../util/Util.h"
#include "audio/AndroidAudioTrackEngine.h"

class AudioBaseEngine;

/* NOTE: the size must be big enough to compensate the hardware audio buffersize size */
/* TODO: We assume that a decoded and resampled frame fits into this buffer */
#define SAMPLE_ARRAY_SIZE (8 * 65536)
/**
 * 一些ff相关的状态和数据。
 */
class VideoState {
public:
    VideoState(std::string& uri,double lastTime = 0);

    ~VideoState();

public:

    void init();

    int stream_component_open(int stream_index,
                              AVDictionary *codec_opts);

    AVDictionary *filter_codec_opts(AVDictionary *opts, enum AVCodecID codec_id,
                                    AVFormatContext *s, AVStream *st, const AVCodec *codec);

    //打开音频暑促设备
    int audio_open(void *opaque, int64_t wanted_channel_layout, int wanted_nb_channels,
                   int wanted_sample_rate, struct AudioParams *audio_hw_params);

    int decoder_init(Decoder *d, AVCodecContext *avctx, PacketQueue *queue);

    int configure_audio_filters(const char *afilters, int force_output_format);

    int configure_filtergraph(AVFilterGraph *graph, const char *filtergraph,
                              AVFilterContext *source_ctx, AVFilterContext *sink_ctx);

    int64_t get_valid_channel_layout(int64_t channel_layout, int channels);

    void step_to_next_frame();

    static void ffLog_callback(void* avcl, int level, const char* fmt, va_list vl);


    void stream_toggle_pause() {
        if (paused) {
            m_videoObj->frame_timer +=
                    av_gettime_relative() / 1000000.0 - m_videoObj->vidclk->last_updated;
            if (read_pause_return != AVERROR(ENOSYS)) {
                m_videoObj->vidclk->paused = 0;
            }
            m_videoObj->vidclk->set_clock(
                    m_videoObj->vidclk->get_clock(), m_videoObj->vidclk->serial);
        }
        m_subtitleObj->extclk->set_clock(m_subtitleObj
                                                 ->extclk->get_clock(),
                                         m_subtitleObj->extclk->serial);
        paused = m_audioObj->audclk->paused = m_videoObj->vidclk->paused = m_subtitleObj->extclk->paused = !paused;
    }

    void stream_seek(int64_t pos, int64_t rel, int seek_by_bytes)
    {
        if (!seek_req) {
            seek_pos = pos;
            seek_rel = rel;
            seek_flags &= ~AVSEEK_FLAG_BYTE;
            if (seek_by_bytes)
                seek_flags |= AVSEEK_FLAG_BYTE;
            seek_req = 1;
//            SDL_CondSignal(continue_read_thread);
        }
    }


public:
    const AVInputFormat *iformat;             // 输入格式
    int abort_request;                          // 请求取消
    int force_refresh;                          // 强制刷新
    int paused;                                 // 停止
    int last_paused;                                // 最后停止
    int queue_attachments_req;          // 队列附件请求
    int seek_req;                                   // 查找请求
    int seek_flags;                             // 查找标志
    int64_t seek_pos;                           // 查找位置
    int64_t seek_rel;                           //
    int read_pause_return;                  // 读停止返回
    AVFormatContext *ic;                    // 解码格式上下文
    int realtime;                                   // 是否实时码流

    VideoObj *m_videoObj;   //视频相关
    AudioObj *m_audioObj;   //音频相关
    SubtitleObj *m_subtitleObj; //字幕相关

    Decoder *auddec;                         // 音频解码器
    Decoder *viddec;                         // 视频解码器
    Decoder *subdec;                         // 字幕解码器


    AVSyncType av_sync_type;                           // 同步类型

    int16_t sample_array[SAMPLE_ARRAY_SIZE]; // 采样数组
    int sample_array_index;                 // 采样索引
    int last_i_start;                               // 上一开始
    RDFTContext *rdft;                      // 自适应滤波器上下文
    int rdft_bits;                                  // 自使用比特率
    FFTSample *rdft_data;                   // 快速傅里叶采样
    int xpos;                                       //
    double last_vis_time;                       //
    //TODO 显示的控件
//    SDL_Texture *vis_texture;               // 音频Texture
//    SDL_Texture *sub_texture;               // 字幕Texture
//    SDL_Texture *vid_texture;               // 视频Texture



    int vfilter_idx;                                // 过滤器索引
    AVFilterGraph *agraph;                  // 音频过滤器 // audio filter graph

    // 上一个视频码流Id、上一个音频码流Id、上一个字幕码流Id
    int last_video_stream, last_audio_stream, last_subtitle_stream;

//    SDL_cond *continue_read_thread; // 连续读线程

    int eof;                                            // 结束标志

    char *afilters = NULL;
    int filter_nbthreads = 0;
    AVDictionary *sws_dict;
    AVDictionary *swr_opts;

    std::string *filename;  // 文件名
    std::string *m_media_title;

};


#endif //ANDROIDTEST_VIDEOSTATE_H
