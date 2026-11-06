//
// Created by Administrator on 2026/11/4.
//

#include "SourceReader.h"

SourceReader::SourceReader(FFContext *ctx) :
        XThread("SourceReader", 50, TIMER),
        ctx(ctx) {



}

SourceReader::~SourceReader() {

}

void SourceReader::onCreate() {
    XThread::onCreate();
    std::ostringstream logOs;
    logOs<<"SourceReader::onCreate";
    st_index.fill(-1);
    pkt = av_packet_alloc();
    if (!pkt) {
        logOs<<"Could not allocate packet.";
    }
    ctx->ic = avformat_alloc_context();
    if (!ctx->ic) {
        logOs<<"Could not allocate context.";

    }
    ctx->ic->interrupt_callback.callback = decode_interrupt_cb;
    ctx->ic->interrupt_callback.opaque = ctx;
    ctx->ic->flags |= AVFMT_FLAG_GENPTS;

    err = avformat_open_input(&ctx->ic, ctx->mURI.c_str(), ctx->iformat, &FFConfig::Instant()->format_opts);
    if (err < 0) {
        logOs<<"err:avformat_open_input"<<err;
        ret = -1;
    }
    //todo check avoptions

    //setup_find_stream_info_opts
    int orig_nb_streams = ctx->ic->nb_streams;
    if (!ctx->ic->nb_streams) {
        logOs<<"Error setting up avformat_find_stream_info() options";
    }

    AVDictionary **opts = static_cast<AVDictionary **>(av_calloc(ctx->ic->nb_streams,
                                                                 sizeof(AVDictionary *)));
    if (!opts) {
        logOs<<"Error av_calloc AVDictionary";
    }
    err = avformat_find_stream_info(ctx->ic, opts);
    if (err < 0) {
        logOs<<"Error could not find codec parameters";
    }




    LOGD("%s",logOs.str().c_str());
}

void SourceReader::onThreadRun(uint32_t now) {

}

void SourceReader::onStop() {
    XThread::onStop();
}

int SourceReader::decode_interrupt_cb(void *ctx) {
    std::ostringstream logOs;
    logOs<<"SourceReader::decode_interrupt_cb";
    LOGD("%s",logOs.str().c_str());

}
