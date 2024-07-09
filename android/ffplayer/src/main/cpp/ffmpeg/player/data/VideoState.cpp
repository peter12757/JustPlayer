//
// Created by Peter Xi on 2022/5/25.
//

#include "VideoState.h"



VideoState::VideoState(std::string& uri,double lastTime)
:iformat(nullptr)
,abort_request(0)
,force_refresh(1)
,paused(0)
,last_paused(0)
,queue_attachments_req(0)
,seek_req(0)
,seek_flags(0)
,seek_pos(0)
,seek_rel(0)
,read_pause_return(0)
,ic(nullptr)
,realtime(0)
, auddec(nullptr)
,viddec(nullptr)
,subdec(nullptr)
, av_sync_type(AV_SYNC_AUDIO_MASTER)
,sample_array_index(0)
,last_i_start(0)
,rdft(nullptr)
,rdft_bits(0)
,rdft_data(nullptr)
,xpos(0)
,last_vis_time(lastTime)
,vfilter_idx(0)
,agraph(nullptr)
,eof(0)
,filename(&uri)
{
    m_videoObj = new VideoObj();
    m_audioObj = new AudioObj();
    m_subtitleObj = new SubtitleObj();

    av_sync_type = AV_SYNC_AUDIO_MASTER;
    //todo state->readThread = ...
}

VideoState::~VideoState() {
    SafeDelete(m_videoObj);
    SafeDelete(m_audioObj);
    SafeDelete(m_subtitleObj);
}

void VideoState::init() {
    av_log_set_level(AV_LOG_DEBUG);
    av_log_set_callback(&VideoState::ffLog_callback);
    avformat_network_init();
    avdevice_register_all();
}

void VideoState::ffLog_callback(void *avcl, int level, const char *fmt, va_list vl) {
    va_list vl2;
    int buf_size = 512;
    std::ostringstream logOs;
    char *line = (char *)malloc(buf_size * sizeof(char));
    static int print_prefix = 1;
    va_copy(vl2, vl);
    av_log_format_line(avcl, level, fmt, vl2, line, buf_size, &print_prefix);
    va_end(vl2);
    line[buf_size-1] = '\0';
    logOs<<"FFPlayer::ffLog_callback "<<line;
    LOGD("%s", logOs.str().c_str());
    SafeDelete(line);
}


void VideoState::step_to_next_frame() {
    /* if the stream is paused unpause it, then step */
    if (paused)
        stream_toggle_pause();
    m_videoObj->step = 1;
}


int64_t VideoState::get_valid_channel_layout(int64_t channel_layout, int channels) {
    if (channel_layout && av_get_channel_layout_nb_channels(channel_layout) == channels)
        return channel_layout;
    else
        return 0;
}

int VideoState::configure_audio_filters(const char *afilters, int force_output_format) {
    static const enum AVSampleFormat sample_fmts[] = { AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_NONE };
    int sample_rates[2] = { 0, -1 };
    int64_t channel_layouts[2] = { 0, -1 };
    int channels[2] = { 0, -1 };
    AVFilterContext *filt_asrc = NULL, *filt_asink = NULL;
    char aresample_swr_opts[512] = "";
    const AVDictionaryEntry *e = NULL;
    char asrc_args[256];
    int ret;

    avfilter_graph_free(&agraph);
    if (!(agraph = avfilter_graph_alloc()))
        return AVERROR(ENOMEM);
    agraph->nb_threads = filter_nbthreads;

    while ((e = av_dict_get(swr_opts, "", e, AV_DICT_IGNORE_SUFFIX)))
        av_strlcatf(aresample_swr_opts, sizeof(aresample_swr_opts), "%s=%s:", e->key, e->value);
    if (strlen(aresample_swr_opts))
        aresample_swr_opts[strlen(aresample_swr_opts)-1] = '\0';
    av_opt_set(agraph, "aresample_swr_opts", aresample_swr_opts, 0);

    ret = snprintf(asrc_args, sizeof(asrc_args),
                   "sample_rate=%d:sample_fmt=%s:channels=%d:time_base=%d/%d",
                   m_audioObj->audio_filter_src.freq, av_get_sample_fmt_name(m_audioObj->audio_filter_src.fmt),
                   m_audioObj->audio_filter_src.channels,
                   1, m_audioObj->audio_filter_src.freq);
//    if (m_audioObj->audio_filter_src.channel_layout)
//        snprintf(asrc_args + ret, sizeof(asrc_args) - ret,
//                 ":channel_layout=0x%"PRIx64,  audio_filter_src.channel_layout);

    ret = avfilter_graph_create_filter(&filt_asrc,
                                       avfilter_get_by_name("abuffer"), "ffplay_abuffer",
                                       asrc_args, NULL, agraph);
    if (ret < 0)
        goto end;


    ret = avfilter_graph_create_filter(&filt_asink,
                                       avfilter_get_by_name("abuffersink"), "ffplay_abuffersink",
                                       NULL, NULL, agraph);
    if (ret < 0)
        goto end;

    if ((ret = av_opt_set_int_list(filt_asink, "sample_fmts", sample_fmts,  AV_SAMPLE_FMT_NONE, AV_OPT_SEARCH_CHILDREN)) < 0)
        goto end;
    if ((ret = av_opt_set_int(filt_asink, "all_channel_counts", 1, AV_OPT_SEARCH_CHILDREN)) < 0)
        goto end;

    if (force_output_format) {
        channel_layouts[0] = m_audioObj->audio_tgt.channel_layout;
        channels       [0] = m_audioObj->audio_tgt.channel_layout ? -1 : m_audioObj->audio_tgt.channels;
        sample_rates   [0] = m_audioObj->audio_tgt.freq;
        if ((ret = av_opt_set_int(filt_asink, "all_channel_counts", 0, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
        if ((ret = av_opt_set_int_list(filt_asink, "channel_layouts", channel_layouts,  -1, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
        if ((ret = av_opt_set_int_list(filt_asink, "channel_counts" , channels       ,  -1, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
        if ((ret = av_opt_set_int_list(filt_asink, "sample_rates"   , sample_rates   ,  -1, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
    }


    if ((ret = configure_filtergraph(agraph, afilters, filt_asrc, filt_asink)) < 0)
        goto end;

    m_audioObj->in_audio_filter  = filt_asrc;
    m_audioObj->out_audio_filter = filt_asink;

    end:
    if (ret < 0)
        avfilter_graph_free(&agraph);
    return ret;
}

int VideoState::stream_component_open(int stream_index, AVDictionary *codec_opts) {

    std::ostringstream logOs;
    logOs<<"VideoState::stream_component_open";
    AVFormatContext *ic = ic;
    AVCodecContext *avctx;
    const AVCodec *codec;
    const char *forced_codec_name = NULL;
    AVDictionary *opts = NULL;
    const AVDictionaryEntry *t = NULL;
    int sample_rate, nb_channels;
    int64_t channel_layout;
    int ret = 0;

    if (stream_index < 0 || stream_index >= ic->nb_streams)
        return -1;

    avctx = avcodec_alloc_context3(NULL);   //AVCodecContext开辟内存并进行简单的初始化
    if (!avctx)
        return AVERROR(ENOMEM);
    //将读取head信息中音频或视频stream中的编解码一些参数赋值给AVCodecContext *avctx
    //这也是我上面一再强调的，编码扩展性，比如从协议mov中读取的stream信息需要重新赋值给上层结构体
    ret = avcodec_parameters_to_context(avctx, ic->streams[stream_index]->codecpar);
    if (ret < 0)
        goto fail;
    avctx->pkt_timebase = ic->streams[stream_index]->time_base;

    //根据head信息中的编解码id,赋值解码器
    codec = avcodec_find_decoder(avctx->codec_id);

    switch(avctx->codec_type){
        case AVMEDIA_TYPE_AUDIO   : last_audio_stream    = stream_index; forced_codec_name = FFConfig::Instant()->audio_codec_name; break;
        case AVMEDIA_TYPE_SUBTITLE: last_subtitle_stream = stream_index; forced_codec_name = FFConfig::Instant()->subtitle_codec_name; break;
        case AVMEDIA_TYPE_VIDEO   : last_video_stream    = stream_index; forced_codec_name = FFConfig::Instant()->video_codec_name; break;
    }
    if (forced_codec_name)
        codec = avcodec_find_decoder_by_name(forced_codec_name);
    if (!codec) {
        if (forced_codec_name) av_log(NULL, AV_LOG_WARNING,
                                      "No codec could be found with name '%s'\n", forced_codec_name);
        else                   av_log(NULL, AV_LOG_WARNING,
                                      "No decoder could be found for codec %s\n", avcodec_get_name(avctx->codec_id));
        ret = AVERROR(EINVAL);
        goto fail;
    }

    avctx->codec_id = codec->id;
    logOs<<"The maximum value for lowres supported by the decoder is "<<codec->max_lowres;
    avctx->lowres = codec->max_lowres;

        avctx->flags2 |= AV_CODEC_FLAG2_FAST;

    opts = filter_codec_opts(codec_opts, avctx->codec_id, ic, ic->streams[stream_index], codec);
    if (!av_dict_get(opts, "threads", NULL, 0))
        av_dict_set(&opts, "threads", "auto", 0);
    //低分辨率？？？
//    av_dict_set_int(&opts, "lowres", stream_lowres, 0);
    if ((ret = avcodec_open2(avctx, codec, &opts)) < 0) {
        goto fail;
    }
    if ((t = av_dict_get(opts, "", NULL, AV_DICT_IGNORE_SUFFIX))) {
        av_log(NULL, AV_LOG_ERROR, "Option %s not found.\n", t->key);
        ret =  AVERROR_OPTION_NOT_FOUND;
        goto fail;
    }

    eof = 0;
    ic->streams[stream_index]->discard = AVDISCARD_DEFAULT;
    switch (avctx->codec_type) {
        case AVMEDIA_TYPE_AUDIO:
            {
            AVFilterContext *sink;

            m_audioObj->audio_filter_src.freq           = avctx->sample_rate;
            m_audioObj->audio_filter_src.channels       = avctx->channels;
            m_audioObj->audio_filter_src.channel_layout = get_valid_channel_layout(avctx->channel_layout, avctx->channels);
            m_audioObj->audio_filter_src.fmt            = avctx->sample_fmt;
            if ((ret = configure_audio_filters(afilters, 0)) < 0)
                goto fail;
            sink = m_audioObj->out_audio_filter;
            sample_rate    = av_buffersink_get_sample_rate(sink);
            nb_channels    = av_buffersink_get_channels(sink);
            channel_layout = av_buffersink_get_channel_layout(sink);
        }
            sample_rate    = avctx->sample_rate;
            nb_channels    = avctx->channels;
            channel_layout = avctx->channel_layout;

            /* prepare audio output */
            if ((ret = audio_open(this,channel_layout, nb_channels, sample_rate, &m_audioObj->audio_tgt)) < 0)
                goto fail;
            m_audioObj->audio_hw_buf_size = ret;
            m_audioObj->audio_src = m_audioObj->audio_tgt;
            m_audioObj->audio_buf_size  = 0;
            m_audioObj->audio_buf_index = 0;

            /* init averaging filter */
            m_audioObj->audio_diff_avg_coef  = exp(log(0.01) / AUDIO_DIFF_AVG_NB);
            m_audioObj->audio_diff_avg_count = 0;
            /* since we do not have a precise anough audio FIFO fullness,
               we correct audio sync only if larger than this threshold */
            m_audioObj->audio_diff_threshold = (double)(m_audioObj->audio_hw_buf_size) / m_audioObj->audio_tgt.bytes_per_sec;

            m_audioObj->audio_stream = stream_index;
            m_audioObj->audio_st = ic->streams[stream_index];

            if ((ret = decoder_init(auddec, avctx, m_audioObj->audioq)) < 0)
                goto fail;
            if ((ic->iformat->flags & (AVFMT_NOBINSEARCH | AVFMT_NOGENSEARCH | AVFMT_NO_BYTE_SEEK)) && !ic->iformat->read_seek) {
                auddec->start_pts = m_audioObj->audio_st->start_time;
                auddec->start_pts_tb = m_audioObj->audio_st->time_base;
            }
            //todo 开始音频解码线程并暂停
//            if ((ret = decoder_start(&auddec, audio_thread, "audio_decoder", is)) < 0)
//                goto out;
//            SDL_PauseAudioDevice(audio_dev, 0);
            break;
        case AVMEDIA_TYPE_VIDEO:
            m_videoObj->video_stream = stream_index;
            m_videoObj->video_st = ic->streams[stream_index];

            if ((ret = decoder_init(viddec, avctx, m_videoObj->videoq)) < 0)
                goto fail;
            //todo 开始视频解码线程
//            if ((ret = decoder_start(&viddec, video_thread, "video_decoder", is)) < 0)
//                goto out;
            queue_attachments_req = 1;
            break;
        case AVMEDIA_TYPE_SUBTITLE:
            m_subtitleObj->subtitle_stream = stream_index;
            m_subtitleObj->subtitle_st = ic->streams[stream_index];

            if ((ret = decoder_init(subdec, avctx, m_subtitleObj->subtitleq)) < 0)
                goto fail;
            //todo 开始字幕的解码线程
//            if ((ret = decoder_start(&subdec, subtitle_thread, "subtitle_decoder", is)) < 0)
//                goto out;
            break;
        default:
            break;
    }
    goto out;

    fail:
    avcodec_free_context(&avctx);
    out:
    av_dict_free(&opts);

    return ret;
}

int VideoState::configure_filtergraph(AVFilterGraph *graph, const char *filtergraph,
                                      AVFilterContext *source_ctx, AVFilterContext *sink_ctx) {
    int ret, i;
    int nb_filters = graph->nb_filters;
    AVFilterInOut *outputs = NULL, *inputs = NULL;

    if (filtergraph) {
        outputs = avfilter_inout_alloc();
        inputs  = avfilter_inout_alloc();
        if (!outputs || !inputs) {
            ret = AVERROR(ENOMEM);
            goto fail;
        }

        outputs->name       = av_strdup("in");
        outputs->filter_ctx = source_ctx;
        outputs->pad_idx    = 0;
        outputs->next       = NULL;

        inputs->name        = av_strdup("out");
        inputs->filter_ctx  = sink_ctx;
        inputs->pad_idx     = 0;
        inputs->next        = NULL;

        if ((ret = avfilter_graph_parse_ptr(graph, filtergraph, &inputs, &outputs, NULL)) < 0)
            goto fail;
    } else {
        if ((ret = avfilter_link(source_ctx, 0, sink_ctx, 0)) < 0)
            goto fail;
    }

    /* Reorder the filters to ensure that inputs of the custom filters are merged first */
    for (i = 0; i < graph->nb_filters - nb_filters; i++)
        FFSWAP(AVFilterContext*, graph->filters[i], graph->filters[i + nb_filters]);

    ret = avfilter_graph_config(graph, NULL);
    fail:
    avfilter_inout_free(&outputs);
    avfilter_inout_free(&inputs);
    return ret;
}

int VideoState::decoder_init(Decoder *d, AVCodecContext *avctx, PacketQueue *queue) {
    memset(d, 0, sizeof(Decoder));
    d->pkt = av_packet_alloc();
    if (!d->pkt)
        return AVERROR(ENOMEM);
    d->avctx = avctx;
    d->queue = queue;
//    d->empty_queue_cond = empty_queue_cond;
    d->start_pts = AV_NOPTS_VALUE;
    d->pkt_serial = -1;
    return 0;
}

AVDictionary *
VideoState::filter_codec_opts(AVDictionary *opts, enum AVCodecID codec_id, AVFormatContext *s,
                              AVStream *st, const AVCodec *codec) {
    AVDictionary    *ret = NULL;
    const AVDictionaryEntry *t = NULL;
    int            flags = s->oformat ? AV_OPT_FLAG_ENCODING_PARAM
                                      : AV_OPT_FLAG_DECODING_PARAM;
    char          prefix = 0;
    const AVClass    *cc = avcodec_get_class();

    if (!codec)
        codec            = s->oformat ? avcodec_find_encoder(codec_id)
                                      : avcodec_find_decoder(codec_id);

    switch (st->codecpar->codec_type) {
        case AVMEDIA_TYPE_VIDEO:
            prefix  = 'v';
            flags  |= AV_OPT_FLAG_VIDEO_PARAM;
            break;
        case AVMEDIA_TYPE_AUDIO:
            prefix  = 'a';
            flags  |= AV_OPT_FLAG_AUDIO_PARAM;
            break;
        case AVMEDIA_TYPE_SUBTITLE:
            prefix  = 's';
            flags  |= AV_OPT_FLAG_SUBTITLE_PARAM;
            break;
    }

    while (t = av_dict_get(opts, "", t, AV_DICT_IGNORE_SUFFIX)) {
        const AVClass *priv_class;
        char *p = strchr(t->key, ':');

        /* check stream specification in opt name */
        if (p)
            switch (avformat_match_stream_specifier(s, st, p + 1)) {
                case  1: *p = 0; break;
                case  0:         continue;
            }

        if (av_opt_find(&cc, t->key, NULL, flags, AV_OPT_SEARCH_FAKE_OBJ) ||
            !codec ||
            ((priv_class = codec->priv_class) &&
             av_opt_find(&priv_class, t->key, NULL, flags,
                         AV_OPT_SEARCH_FAKE_OBJ)))
            av_dict_set(&ret, t->key, t->value, 0);
        else if (t->key[0] == prefix &&
                 av_opt_find(&cc, t->key + 1, NULL, flags,
                             AV_OPT_SEARCH_FAKE_OBJ))
            av_dict_set(&ret, t->key + 1, t->value, 0);

        if (p)
            *p = ':';
    }
    return ret;
}


int VideoState::audio_open(void *opaque, int64_t wanted_channel_layout, int wanted_nb_channels,
                           int wanted_sample_rate, struct AudioParams *audio_hw_params) {
//    SDL_AudioSpec wanted_spec, spec;
//    const char *env;
//    static const int next_nb_channels[] = {0, 0, 1, 6, 2, 6, 4, 6};
//    static const int next_sample_rates[] = {0, 44100, 48000, 96000, 192000};
//    int next_sample_rate_idx = FF_ARRAY_ELEMS(next_sample_rates) - 1;
//
//    env = SDL_getenv("SDL_AUDIO_CHANNELS");
//    if (env) {
//        wanted_nb_channels = atoi(env);
//        wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
//    }
//    if (!wanted_channel_layout || wanted_nb_channels != av_get_channel_layout_nb_channels(wanted_channel_layout)) {
//        wanted_channel_layout = av_get_default_channel_layout(wanted_nb_channels);
//        wanted_channel_layout &= ~AV_CH_LAYOUT_STEREO_DOWNMIX;
//    }
//    wanted_nb_channels = av_get_channel_layout_nb_channels(wanted_channel_layout);
//    wanted_spec.channels = wanted_nb_channels;
//    wanted_spec.freq = wanted_sample_rate;
//    if (wanted_spec.freq <= 0 || wanted_spec.channels <= 0) {
//        av_log(NULL, AV_LOG_ERROR, "Invalid sample rate or channel count!\n");
//        return -1;
//    }
//    while (next_sample_rate_idx && next_sample_rates[next_sample_rate_idx] >= wanted_spec.freq)
//        next_sample_rate_idx--;
//    wanted_spec.format = AUDIO_S16SYS;
//    wanted_spec.silence = 0;
//    wanted_spec.samples = FFMAX(SDL_AUDIO_MIN_BUFFER_SIZE, 2 << av_log2(wanted_spec.freq / SDL_AUDIO_MAX_CALLBACKS_PER_SEC));
//    wanted_spec.callback = sdl_audio_callback;
//    wanted_spec.userdata = opaque;
//    while (!(audio_dev = SDL_OpenAudioDevice(NULL, 0, &wanted_spec, &spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE))) {
//        av_log(NULL, AV_LOG_WARNING, "SDL_OpenAudio (%d channels, %d Hz): %s\n",
//               wanted_spec.channels, wanted_spec.freq, SDL_GetError());
//        wanted_spec.channels = next_nb_channels[FFMIN(7, wanted_spec.channels)];
//        if (!wanted_spec.channels) {
//            wanted_spec.freq = next_sample_rates[next_sample_rate_idx--];
//            wanted_spec.channels = wanted_nb_channels;
//            if (!wanted_spec.freq) {
//                av_log(NULL, AV_LOG_ERROR,
//                       "No more combinations to try, audio open failed\n");
//                return -1;
//            }
//        }
//        wanted_channel_layout = av_get_default_channel_layout(wanted_spec.channels);
//    }
//    if (spec.format != AUDIO_S16SYS) {
//        av_log(NULL, AV_LOG_ERROR,
//               "SDL advised audio format %d is not supported!\n", spec.format);
//        return -1;
//    }
//    if (spec.channels != wanted_spec.channels) {
//        wanted_channel_layout = av_get_default_channel_layout(spec.channels);
//        if (!wanted_channel_layout) {
//            av_log(NULL, AV_LOG_ERROR,
//                   "SDL advised channel count %d is not supported!\n", spec.channels);
//            return -1;
//        }
//    }
//
//    audio_hw_params->fmt = AV_SAMPLE_FMT_S16;
//    audio_hw_params->freq = spec.freq;
//    audio_hw_params->channel_layout = wanted_channel_layout;
//    audio_hw_params->channels =  spec.channels;
//    audio_hw_params->frame_size = av_samples_get_buffer_size(NULL, audio_hw_params->channels, 1, audio_hw_params->fmt, 1);
//    audio_hw_params->bytes_per_sec = av_samples_get_buffer_size(NULL, audio_hw_params->channels, audio_hw_params->freq, audio_hw_params->fmt, 1);
//    if (audio_hw_params->bytes_per_sec <= 0 || audio_hw_params->frame_size <= 0) {
//        av_log(NULL, AV_LOG_ERROR, "av_samples_get_buffer_size failed\n");
//        return -1;
//    }
//    return spec.size;
    return -1;
}





