//
// Created by Administrator on 2024/9/23.
//

#include "VideoThread.h"


VideoThread::VideoThread(VideoState *is,int stream_index,int stream_lowers)
:StreamThread(is,stream_index,stream_lowers)
{
     std::ostringstream log;
     if (stream_index <0 || stream_index>= mediaState->ic->nb_streams) {
         log<<"stream_index error:"<<stream_index;
         LOGE("%s",log.str().c_str());
     }
     avctx = avcodec_alloc_context3(NULL);
     if (!avctx) {
         log<<"avctx error:"<< AVERROR(ENOMEM);
         LOGE("%s",log.str().c_str());
     }
     err_code = avcodec_parameters_to_context(avctx,mediaState->ic->streams[stream_index]->codecpar);
     if (err_code <0) {
         log<<"avcodec_parameters_to_context error";
         LOGE("%s",log.str().c_str());
     }
     avctx->pkt_timebase = mediaState->ic->streams[stream_index]->time_base;
     codec = avcodec_find_decoder(avctx->codec_id);

     mediaState->last_video_stream = stream_index;
     forced_codec_name = mediaState->video_codec_name;

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
    if (stream_lowers)
        av_dict_set_int(&opts, "lowres", stream_lowers, 0);

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
    mediaState






    LOGD("%s",log.str().c_str());


}

VideoThread::~VideoThread() {

}

void VideoThread::onCreate() {


}

void VideoThread::onThreadRun(uint32_t now) {

}

void VideoThread::onStop() {

}

int VideoThread::create_hwaccel(AVBufferRef **device_ctx) {
    enum AVHWDeviceType type;
    int ret;
    AVBufferRef *vk_dev;

    *device_ctx = NULL;

    if (!hwaccel)
        return 0;

    type = av_hwdevice_find_type_by_name(hwaccel);
    if (type == AV_HWDEVICE_TYPE_NONE)
        return AVERROR(ENOTSUP);

    ret = vk_renderer_get_hw_dev(vk_renderer, &vk_dev);
    if (ret < 0)
        return ret;

    ret = av_hwdevice_ctx_create_derived(device_ctx, type, vk_dev, 0);
    if (!ret)
        return 0;

    if (ret != AVERROR(ENOSYS))
        return ret;

    av_log(NULL, AV_LOG_WARNING, "Derive %s from vulkan not supported.\n", hwaccel);
    ret = av_hwdevice_ctx_create(device_ctx, type, NULL, NULL, 0);
    return ret;
}
