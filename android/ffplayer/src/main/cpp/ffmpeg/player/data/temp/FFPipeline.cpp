//
// Created by Administrator on 2024/7/28.
//

#include "FFPipeline.h"

FFPipeline::FFPipeline(string opaque_class, size_t opaque_size):
opaque_class(opaque_class){
    opaque = (FFPlayer*)calloc(1,opaque_size);
    if (!opaque) {
        LOGE("opaque calloc ERR");
    }
}

FFPipeline::~FFPipeline() {
    func_destroy();
    SafeDelete(opaque);
}

FFPipenode *FFPipeline::ffpipeline_open_video_decoder(FFPlayer *ffp) {
    return func_open_video_decoder(ffp);
}

Aout *FFPipeline::ffpipeline_open_audio_output(FFPlayer *ffp) {
    return func_open_audio_output(ffp);
}




