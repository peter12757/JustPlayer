//
// Created by Administrator on 2024/9/25.
//

#include "StreamThread.h"




StreamThread::StreamThread(VideoState *is,int stream_index,int stream_lowers)
{
    std::ostringstream log;
    log<<"VideoThread::VideoThread";
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
}

StreamThread::~StreamThread() {

}

void StreamThread::onCreate() {


}

void StreamThread::onThreadRun(uint32_t now) {

}

void StreamThread::onStop() {

}

int StreamThread::get_master_sync_type() {
    if (mediaState->av_sync_type == AV_SYNC_VIDEO_MASTER) {
        if (mediaState->video_st)
            return AV_SYNC_VIDEO_MASTER;
        else
            return AV_SYNC_AUDIO_MASTER;
    } else if (mediaState->av_sync_type == AV_SYNC_AUDIO_MASTER) {
        if (mediaState->audio_st)
            return AV_SYNC_AUDIO_MASTER;
        else
            return AV_SYNC_EXTERNAL_CLOCK;
    } else {
        return AV_SYNC_EXTERNAL_CLOCK;
    }
    return 0;
}

double StreamThread::get_master_clock() {
    Clock *clk;

    switch (get_master_sync_type()) {
        case AV_SYNC_VIDEO_MASTER:
            clk = mediaState->vidclk;
            break;
        case AV_SYNC_AUDIO_MASTER:
            clk = mediaState->audclk;
            break;
        default:
            clk = mediaState->extclk;
            break;
    }
    return clk->get_clock();
}


