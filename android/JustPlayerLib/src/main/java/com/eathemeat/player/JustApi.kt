package com.eathemeat.player

import android.view.Surface

class JustApi {

    /**
     * A native method that is implemented by the 'ffmpeglib' native library,
     * which is packaged with this application.
     */
    external fun createPlayer(): Long

    external fun destory(player:Long): Unit
    
    external fun invoke(handle:Long,data: ByteArray): Int
    external fun setSurface(handle:Long,it: Surface?)
    external fun setCallBack(handle:Long,callback:JustCallBack): Unit

    external fun testmain(name:ByteArray,desc:ByteArray):Unit

    companion object {
        init {
            System.loadLibrary("JustApi")
            System.loadLibrary("swscale")
            System.loadLibrary("avformat")
            System.loadLibrary("avdevice")
            System.loadLibrary("avfilter")
            System.loadLibrary("avutil")
            System.loadLibrary("avcodec")
            System.loadLibrary("swresample")
            System.loadLibrary("JUSTPlayer")
            System.loadLibrary("CPPTest")
        }
    }






}