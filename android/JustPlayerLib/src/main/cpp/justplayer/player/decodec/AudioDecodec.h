//
// Created by Peter Xi on 2022/6/5.
//

#ifndef ANDROIDTEST_AUDIODECODEC_H
#define ANDROIDTEST_AUDIODECODEC_H

#include "../data/FFinc.h"
#include "../data/VideoState.h"

class AudioDecodec : XThread {


    typedef struct BufferSinkContext {
        const AVClass *avclass;
        unsigned warning_limit;

        /* only used for video */
        enum AVPixelFormat *pixel_fmts;     ///< list of accepted pixel formats
        int pixel_fmts_size;

        /* only used for audio */
        enum AVSampleFormat *sample_fmts;   ///< list of accepted sample formats
        int sample_fmts_size;
        int64_t *channel_layouts;           ///< list of accepted channel layouts
        int channel_layouts_size;
        int *channel_counts;                ///< list of accepted channel counts
        int channel_counts_size;
        int all_channel_counts;
        int *sample_rates;                  ///< list of accepted sample rates
        int sample_rates_size;

        AVFrame *peeked_frame;
    } BufferSinkContext;

public:
    AudioDecodec(VideoState *vs);

    virtual ~AudioDecodec();

    virtual void onThreadRun(uint32_t now) override;

    int decoder_decode_frame(Decoder *d, AVSubtitle *sub);

    /* return < 0 if aborted, 0 if no packet and > 0 if packet.  */
    int packet_queue_get(PacketQueue *q, AVPacket *pkt, int block, int *serial);

    int cmp_audio_fmts(enum AVSampleFormat fmt1, int64_t channel_count1,
                       enum AVSampleFormat fmt2, int64_t channel_count2);

    int configure_audio_filters(VideoState *is, const char *afilters, int force_output_format);

    int configure_filtergraph(AVFilterGraph *graph, const char *filtergraph,
                              AVFilterContext *source_ctx, AVFilterContext *sink_ctx);


    Frame *frame_queue_peek_writable(FrameQueue *f);





public:
    VideoState *is;
    AVFrame *frame;
    Frame *af;

    //avfliter
    int last_serial = -1;
    int64_t dec_channel_layout;
    int reconfigure;

    int got_frame = 0;
    AVRational tb;

    int decoder_reorder_pts = -1;

    AVDictionary *swr_opts;
    int filter_nbthreads = 0;   //number of filter threads per graph
    char *afilters = NULL;     //set audio filters", "filter_graph"
};


#endif //ANDROIDTEST_AUDIODECODEC_H
