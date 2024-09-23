//
// Created by Administrator on 2024/8/5.
//

#include "VideoState.h"

#include <utility>

VideoState::VideoState(MyFFPlayer * player,std::string url,AVInputFormat *iformat,int av_sync_type)
:player(player)
,datasourcel(url)
,video_stream(-1)
,last_video_stream(-1)
,pictq_size(VIDEO_PICTURE_QUEUE_SIZE_DEFAULT)
,audio_stream(-1)
,last_audio_stream(-1)
,subtitle_stream(-1)
,last_subtitle_stream(-1)
,eof(0)
,error(0)
,iformat(iformat)
,ytop(0)
,xleft(0)
,audio_clock_serial(-1)
,audio_volume(SDL_MIX_MAXVOLUME)
,muted(false)
,volume(0)
,av_sync_type(AV_SYNC_AUDIO_MASTER)
{
    ic = avformat_alloc_context();
    if (!ic) {
        LOGE("ReadThread::onCreate ERROR");
    }
    ic->interrupt_callback.callback = decode_interrupt_cb;
    ic->interrupt_callback.opaque = this;

    videoq = new PacketQueue(true);
    subtitleq = new PacketQueue(true);
    audioq = new PacketQueue(true);

    //video
    video_fq = new FrameQueue(videoq,pictq_size,1);
 //
    audio_fq = new FrameQueue(audioq,SAMPLE_QUEUE_SIZE,0);

    subpq = new FrameQueue(subtitleq,SUBPICTURE_QUEUE_SIZE,0);

    if (!video_fq->isvalid || !video_fq->isvalid || !video_fq->isvalid ) {
        LOGE("VideoState CREATE FAIL");
        abort_request =true;
        error = -1;
    }

#if CONFIG_AVFILTER
    if (vfilter0) {
        GROW_ARRAY(vfilters_list, nb_vfilters);
        vfilters_list[nb_vfilters - 1] = vfilter0;
    }
#endif

    vidclk = new Clock(&videoq->serial);
    audclk = new Clock(&audioq->serial);
    extclk = new Clock(&extclk->serial);
    pause_req = !start_on_prepared;

}

VideoState::~VideoState() {

}
