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
    ic = avformat_alloc_context();
    if (!ic) {
        logOs<<"Could not allocate context.";

    }
    ic->interrupt_callback.callback = decode_interrupt_cb;
    ic->interrupt_callback.opaque = ctx;

    err = avformat_open_input(&ic, ctx->mURI.c_str(), ctx->iformat, &FFConfig::Instant()->format_opts);
    if (err < 0) {
        logOs<<"err:avformat_open_input"<<err;
        ret = -1;
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
