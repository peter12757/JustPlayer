package com.eathemeat.ffplayer

class NativeLib {

    /**
     * A native method that is implemented by the 'ffplayer' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    companion object {
        // Used to load the 'ffplayer' library on application startup.
        init {
            System.loadLibrary("ffplayer")
        }
    }
}