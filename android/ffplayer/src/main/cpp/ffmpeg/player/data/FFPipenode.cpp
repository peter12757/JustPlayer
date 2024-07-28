//
// Created by Administrator on 2024/7/28.
//

#include "FFPipenode.h"

FFPipenode::FFPipenode(int opaque_size) {

    opaque = calloc(1, opaque_size);
    if (!opaque) {
        LOGE("opaque calloc ERR")
    }

//    mutex = SDL_CreateMutex();
//    if (mutex == NULL) {
//        free(opaque);
//        free(node);
//        return NULL;
//    }
}

FFPipenode::~FFPipenode() {
    func_destroy();
//    SDL_DestroyMutexP(&mutex);
    SafeDelete(opaque);
}


