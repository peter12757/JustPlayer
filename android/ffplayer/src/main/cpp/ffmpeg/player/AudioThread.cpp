//
// Created by Administrator on 2024/9/23.
//


#include "AudioThread.h"

AudioThread::AudioThread(VideoState *is,int stream_index,int stream_lowers)
        : StreamThread(is,stream_index,stream_lowers)
        {
    std::ostringstream log;
    log<<"VideoThread::VideoThread ";
    frame = av_frame_alloc();
            if (!frame) {
                log<<"frame is null";
                LOGE("%s",log.str().c_str());
            }
            mediaState->last_audio_stream = stream_index;
            forced_codec_name = mediaState->audio_codec_name;



}

AudioThread::~AudioThread() {

}

void AudioThread::onCreate() {
    std::ostringstream log;
    log<<"AudioThread::onCreate";
    if (!forced_codec_name.empty()) {
        codec = avcodec_find_decoder_by_name(forced_codec_name.c_str());
    }
    if (!codec) {
        if (forced_codec_name.empty()) {
            log<<"No decoder could be found for codec "<<avcodec_get_name(avctx->codec_id);
        }else {
            log<<"No codec could be found with name "<<forced_codec_name;
        }
        log<< AVERROR(EINVAL);
        LOGE("%s",log.str().c_str());
    }
    avctx->codec_id = codec->id;
    if (stream_lowers > codec->max_lowres) {
        log<<"The maximum value for lowres supported by the decoder is "<<codec->max_lowres<<"\n";
        stream_lowers = codec->max_lowres;
    }
    avctx->lowres = stream_lowers;
    if (mediaState->fast) {
        avctx->flags2 |= AV_CODEC_FLAG2_FAST;
    }
    err_code = filter_codec_opts(mediaState->codec_opts, avctx->codec_id, mediaState->ic,
                                 mediaState->ic->streams[stream_index], codec, &opts);
    if (err_code < 0) {
        log<<"filter_codec_opts error";
        LOGE("%s",log.str().c_str());
    }
    if (!av_dict_get(opts, "threads", nullptr, 0))
        av_dict_set(&opts, "threads", "auto", 0);
    if (stream_lowers) {
        av_dict_set_int(&opts, "lowres", stream_lowers, 0);
    }
    av_dict_set(&opts, "flags", "+copy_opaque", AV_DICT_MULTIKEY);

    err_code = create_hwaccel(&avctx->hw_device_ctx);
    if (err_code < 0) {
        log<<"create_hwaccel error";
        LOGE("%s",log.str().c_str());
    }

    if ((err_code = avcodec_open2(avctx, codec, &opts)) < 0) {
        log<<"avcodec_open2 error";
        LOGE("%s",log.str().c_str());
    }
    if ((t = av_dict_get(opts, "", NULL, AV_DICT_IGNORE_SUFFIX))) {
        av_log(NULL, AV_LOG_ERROR, "Option %s not found.\n", t->key);
        err_code =  AVERROR_OPTION_NOT_FOUND;
        log<<"AVERROR_OPTION_NOT_FOUND error";
        LOGE("%s",log.str().c_str());
    }


    mediaState->eof = 0;
    mediaState->ic->streams[stream_index]->discard = AVDISCARD_DEFAULT;
    mediaState->video_stream = stream_index;
    mediaState->video_st = mediaState->ic->streams[stream_index];
    mediaState->viddec = new Decoder(avctx,mediaState->videoq);
    LOGD("%s",log.str().c_str());

    //audio type
    AVFilterContext *sink;

    mediaState->audio_filter_src->freq = avctx->sample_rate;
    err_code = av_channel_layout_copy(&mediaState->audio_filter_src->ch_layout, &avctx->ch_layout);
    if (err_code < 0) {
        log<<"av_channel_layout_copy err";
        LOGE("%s",log.str().c_str());
    }
    mediaState->audio_filter_src->fmt = avctx->sample_fmt;
    if ((err_code = configure_audio_filters(afilters, 0)) < 0) {
        log<<"configure_audio_filters error"<<err_code;
        LOGE("%s",log.str().c_str());
    }

    sink = mediaState->out_audio_filter;
    sample_rate    = av_buffersink_get_sample_rate(sink);
    err_code = av_buffersink_get_ch_layout(sink, &ch_layout);
    if (err_code < 0) {
        log<<"av_buffersink_get_ch_layout error"<<err_code;
        LOGE("%s",log.str().c_str());
    }
    /* prepare audio output */
    if ((err_code = audio_open(mediaState, &ch_layout, sample_rate, &mediaState->audio_tgt)) < 0) {
        log<<"audio_open error"<<err_code;
        LOGE("%s",log.str().c_str());
    }
    mediaState->audio_hw_buf_size = ret;
    mediaState->audio_src = mediaState->audio_tgt;
    mediaState->audio_buf_size  = 0;
    mediaState->audio_buf_index = 0;

    /* init averaging filter */
    mediaState->audio_diff_avg_coef  = exp(log(0.01) / AUDIO_DIFF_AVG_NB);
    mediaState->audio_diff_avg_count = 0;
    /* since we do not have a precise anough audio FIFO fullness,
       we correct audio sync only if larger than this threshold */
    mediaState->audio_diff_threshold = (double)(mediaState->audio_hw_buf_size) / mediaState->audio_tgt.bytes_per_sec;

    mediaState->audio_stream = stream_index;
    mediaState->audio_st = ic->streams[stream_index];

    if ((ret = decoder_init(&mediaState->auddec, avctx, &mediaState->audioq, mediaState->continue_read_thread)) < 0)
        goto fail;
    if (mediaState->ic->iformat->flags & AVFMT_NOTIMESTAMPS) {
        mediaState->auddec.start_pts = mediaState->audio_st->start_time;
        mediaState->auddec.start_pts_tb = mediaState->audio_st->time_base;
    }
}

void AudioThread::onThreadRun(uint32_t now) {

}

void AudioThread::onStop() {

}

int AudioThread::configure_audio_filters(const std::stringafilters, int force_output_format) {
    enum AVSampleFormat sample_fmts[] = { AV_SAMPLE_FMT_S16, AV_SAMPLE_FMT_NONE };
    int sample_rates[2] = { 0, -1 };
    AVFilterContext *filt_asrc = NULL, *filt_asink = NULL;
    char aresample_swr_opts[512] = "";
    const AVDictionaryEntry *e = NULL;
    AVBPrint bp;
    char asrc_args[256];
    int ret;

    avfilter_graph_free(&mediaState->agraph);
    if (!(mediaState->agraph = avfilter_graph_alloc()))
        return AVERROR(ENOMEM);
    mediaState->agraph->nb_threads = filter_nbthreads;

    av_bprint_init(&bp, 0, AV_BPRINT_SIZE_AUTOMATIC);

    while ((e = av_dict_iterate(swr_opts, e)))
        av_strlcatf(aresample_swr_opts, sizeof(aresample_swr_opts), "%s=%s:", e->key, e->value);
    if (strlen(aresample_swr_opts))
        aresample_swr_opts[strlen(aresample_swr_opts)-1] = '\0';
    av_opt_set(mediaState->agraph, "aresample_swr_opts", aresample_swr_opts, 0);

    av_channel_layout_describe_bprint(&mediaState->audio_filter_src.ch_layout, &bp);

    ret = snprintf(asrc_args, sizeof(asrc_args),
                   "sample_rate=%d:sample_fmt=%s:time_base=%d/%d:channel_layout=%s",
                   mediaState->audio_filter_src.freq, av_get_sample_fmt_name(mediaState->audio_filter_src.fmt),
                   1, mediaState->audio_filter_src.freq, bp.str);

    ret = avfilter_graph_create_filter(&filt_asrc,
                                       avfilter_get_by_name("abuffer"), "ffplay_abuffer",
                                       asrc_args, NULL, mediaState->agraph);
    if (ret < 0)
        goto end;


    ret = avfilter_graph_create_filter(&filt_asink,
                                       avfilter_get_by_name("abuffersink"), "ffplay_abuffersink",
                                       NULL, NULL, mediaState->agraph);
    if (ret < 0)
        goto end;

    if ((ret = av_opt_set_int_list(filt_asink, "sample_fmts", sample_fmts,  AV_SAMPLE_FMT_NONE, AV_OPT_SEARCH_CHILDREN)) < 0)
        goto end;
    if ((ret = av_opt_set_int(filt_asink, "all_channel_counts", 1, AV_OPT_SEARCH_CHILDREN)) < 0)
        goto end;

    if (force_output_format) {
        av_bprint_clear(&bp);
        av_channel_layout_describe_bprint(&mediaState->audio_tgt.ch_layout, &bp);
        sample_rates   [0] = mediaState->audio_tgt.freq;
        if ((ret = av_opt_set_int(filt_asink, "all_channel_counts", 0, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
        if ((ret = av_opt_set(filt_asink, "ch_layouts", bp.str, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
        if ((ret = av_opt_set_int_list(filt_asink, "sample_rates"   , sample_rates   ,  -1, AV_OPT_SEARCH_CHILDREN)) < 0)
            goto end;
    }


    if ((ret = configure_filtergraph(mediaState->agraph, afilters, filt_asrc, filt_asink)) < 0)
        goto end;

    mediaState->in_audio_filter  = filt_asrc;
    mediaState->out_audio_filter = filt_asink;

    end:
    if (ret < 0)
        avfilter_graph_free(&mediaState->agraph);
    av_bprint_finalize(&bp, NULL);

    return ret;
    return 0;
}

int AudioThread::audio_open(void *opaque, AVChannelLayout *wanted_channel_layout,
                            int wanted_sample_rate, struct AudioParams *audio_hw_params) {
    SDL_AudioSpec wanted_spec, spec;
    const char *env;
    static const int next_nb_channels[] = {0, 0, 1, 6, 2, 6, 4, 6};
    static const int next_sample_rates[] = {0, 44100, 48000, 96000, 192000};
    int next_sample_rate_idx = FF_ARRAY_ELEMS(next_sample_rates) - 1;
    int wanted_nb_channels = wanted_channel_layout->nb_channels;

    env = SDL_getenv("SDL_AUDIO_CHANNELS");
    if (env) {
        wanted_nb_channels = atoi(env);
        av_channel_layout_uninit(wanted_channel_layout);
        av_channel_layout_default(wanted_channel_layout, wanted_nb_channels);
    }
    if (wanted_channel_layout->order != AV_CHANNEL_ORDER_NATIVE) {
        av_channel_layout_uninit(wanted_channel_layout);
        av_channel_layout_default(wanted_channel_layout, wanted_nb_channels);
    }
    wanted_nb_channels = wanted_channel_layout->nb_channels;
    wanted_spec.channels = wanted_nb_channels;
    wanted_spec.freq = wanted_sample_rate;
    if (wanted_spec.freq <= 0 || wanted_spec.channels <= 0) {
        av_log(NULL, AV_LOG_ERROR, "Invalid sample rate or channel count!\n");
        return -1;
    }
    while (next_sample_rate_idx && next_sample_rates[next_sample_rate_idx] >= wanted_spec.freq)
        next_sample_rate_idx--;
    wanted_spec.format = AUDIO_S16SYS;
    wanted_spec.silence = 0;
    wanted_spec.samples = FFMAX(SDL_AUDIO_MIN_BUFFER_SIZE, 2 << av_log2(wanted_spec.freq / SDL_AUDIO_MAX_CALLBACKS_PER_SEC));
    wanted_spec.callback = sdl_audio_callback;
    wanted_spec.userdata = opaque;
    while (!(audio_dev = SDL_OpenAudioDevice(NULL, 0, &wanted_spec, &spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE | SDL_AUDIO_ALLOW_CHANNELS_CHANGE))) {
        av_log(NULL, AV_LOG_WARNING, "SDL_OpenAudio (%d channels, %d Hz): %s\n",
               wanted_spec.channels, wanted_spec.freq, SDL_GetError());
        wanted_spec.channels = next_nb_channels[FFMIN(7, wanted_spec.channels)];
        if (!wanted_spec.channels) {
            wanted_spec.freq = next_sample_rates[next_sample_rate_idx--];
            wanted_spec.channels = wanted_nb_channels;
            if (!wanted_spec.freq) {
                av_log(NULL, AV_LOG_ERROR,
                       "No more combinations to try, audio open failed\n");
                return -1;
            }
        }
        av_channel_layout_default(wanted_channel_layout, wanted_spec.channels);
    }
    if (spec.format != AUDIO_S16SYS) {
        av_log(NULL, AV_LOG_ERROR,
               "SDL advised audio format %d is not supported!\n", spec.format);
        return -1;
    }
    if (spec.channels != wanted_spec.channels) {
        av_channel_layout_uninit(wanted_channel_layout);
        av_channel_layout_default(wanted_channel_layout, spec.channels);
        if (wanted_channel_layout->order != AV_CHANNEL_ORDER_NATIVE) {
            av_log(NULL, AV_LOG_ERROR,
                   "SDL advised channel count %d is not supported!\n", spec.channels);
            return -1;
        }
    }

    audio_hw_params->fmt = AV_SAMPLE_FMT_S16;
    audio_hw_params->freq = spec.freq;
    if (av_channel_layout_copy(&audio_hw_params->ch_layout, wanted_channel_layout) < 0)
        return -1;
    audio_hw_params->frame_size = av_samples_get_buffer_size(NULL, audio_hw_params->ch_layout.nb_channels, 1, audio_hw_params->fmt, 1);
    audio_hw_params->bytes_per_sec = av_samples_get_buffer_size(NULL, audio_hw_params->ch_layout.nb_channels, audio_hw_params->freq, audio_hw_params->fmt, 1);
    if (audio_hw_params->bytes_per_sec <= 0 || audio_hw_params->frame_size <= 0) {
        av_log(NULL, AV_LOG_ERROR, "av_samples_get_buffer_size failed\n");
        return -1;
    }
    return spec.size;
    return 0;
}
