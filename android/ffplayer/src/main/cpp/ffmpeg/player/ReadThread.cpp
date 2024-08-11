//
// Created by PeterXi on 2024/8/10.
//

#include "ReadThread.h"

ReadThread::ReadThread(VideoState *is)
        : XThread("ReadThread", 10, URGENT)
        ,mediaState(is){

}

ReadThread::~ReadThread() {

}

void ReadThread::onThreadRun(uint32_t now) {



}

void ReadThread::onCreate() {
    XThread::onCreate();
    mediaState->last_video_stream = mediaState->video_stream = -1;
    mediaState->last_audio_stream = mediaState->audio_stream = -1;
    mediaState->last_subtitle_stream = mediaState->subtitle_stream = -1;
    mediaState->eof = 0;

    mediaState->ic = avformat_alloc_context();
    if (!mediaState->ic) {
        LOGE("ReadThread::onCreate ERROR");
    }
    mediaState->ic->interrupt_callback.callback = this->decode_interrupt_cb;
    mediaState->ic->interrupt_callback.opaque = mediaState;

    //format
    if (!av_dict_get(mediaState->format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE)) {
        av_dict_set(&mediaState->format_opts, "scan_all_pmts", "1", AV_DICT_DONT_OVERWRITE);
        scan_all_pmts_set = 1;
    }
    if (av_stristart(mediaState->filename.c_str(), "rtmp", NULL) ||
        av_stristart(mediaState->filename.c_str(), "rtsp", NULL)) {
        // There is total different meaning for 'timeout' option in rtmp
        av_log(mediaState, AV_LOG_WARNING, "remove 'timeout' option for rtmp.\n");
        av_dict_set(&mediaState->format_opts, "timeout", NULL, 0);
    }

    //other format name???todo
    errCode = avformat_open_input(&mediaState->ic,mediaState->filename.c_str(),mediaState->iformat,&mediaState->format_opts);
    if (errCode < 0) {
        LOGE("ReadThread::onCreate avformat_open_input err");
    }

    if (scan_all_pmts_set)
        av_dict_set(&mediaState->format_opts, "scan_all_pmts", NULL, AV_DICT_MATCH_CASE);

    if ((t = av_dict_get(mediaState->format_opts, "", NULL, AV_DICT_IGNORE_SUFFIX))) {
        av_log(NULL, AV_LOG_ERROR, "Option %s not found.\n", t->key);
#ifdef FFP_MERGE
        ret = AVERROR_OPTION_NOT_FOUND;
#endif
    }



}

void ReadThread::onStop() {
    XThread::onStop();
}
