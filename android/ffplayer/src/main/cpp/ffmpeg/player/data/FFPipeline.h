//
// Created by Administrator on 2024/7/28.
//

#ifndef JUSTPLAYER_FFPIPELINE_H
#define JUSTPLAYER_FFPIPELINE_H

#include "FFPlayer.h"
#include "FFPipenode.h"
#include "LogUtil.h"


class FFPipeline {
public:
    string opaque_class;
    FFPlayer* opaque = nullptr;

    FFPipeline(string opaque_class, size_t opaque_size);
    ~FFPipeline();

    virtual void *func_destroy() = 0;
    virtual FFPipenode* func_open_video_decoder(FFPlayer *ffp) = 0;
    virtual Aout* func_open_audio_output(FFPlayer *ffp) = 0;


    FFPipenode* ffpipeline_open_video_decoder(FFPlayer *ffp);
    Aout* ffpipeline_open_audio_output(FFPlayer *ffp);
};


#endif //JUSTPLAYER_FFPIPELINE_H
