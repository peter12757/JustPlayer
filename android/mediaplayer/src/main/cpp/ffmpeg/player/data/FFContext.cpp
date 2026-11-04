//
// Created by Administrator on 2026/10/27.
//

#include "FFContext.h"


FFContext::FFContext(string uri):
mURI(uri){
    videoState = new VideoState();
    audioState = new AudioState();
    meta = new MediaMeta();

    file_iformat = av_find_input_format(mURI.c_str());


}

FFContext::~FFContext() {
    SafeDelete(videoState);
    SafeDelete(audioState);
    SafeDelete(meta);
}


