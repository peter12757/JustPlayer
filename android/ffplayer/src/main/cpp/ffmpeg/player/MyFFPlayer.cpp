//
// Created by Administrator on 2024/8/4.
//


#include "MyFFPlayer.h"




MyFFPlayer::MyFFPlayer() {
    videObj = new VideoObj();
}

MyFFPlayer::~MyFFPlayer() {
    SafeDelete(videObj);
}

int MyFFPlayer::setSurface(ANativeWindow *native_window) {
    std::ostringstream logOs;
    logOs<< "MyFFPlayer::setSurface";
    //todo mutex?
    if (native_window == videObj->native_window) {
        if (!native_window) {
            logOs<<"native_window is null reset";
            videObj->resetAllMediaCodecBuffers();
        }
        return 1;
    }
//    IJK_EGL_terminate(opaque->egl);   //todo egl renders
    videObj->resetAllMediaCodecBuffers();

    if (videObj->native_window)
        ANativeWindow_release(videObj->native_window);

    if (native_window)
        ANativeWindow_acquire(native_window);

    videObj->native_window = native_window;
    videObj->null_native_window_warned = 0;
    return 1;
}
