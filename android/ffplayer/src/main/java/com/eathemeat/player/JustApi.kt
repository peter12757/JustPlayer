package com.eathemeat.player

import android.view.Surface

class JustApi {

    /**
     * A native method that is implemented by the 'ffmpeglib' native library,
     * which is packaged with this application.
     */
//    external fun createPlayer(context: ByteArray): Long
    fun createPlayer(context: ByteArray): Long {


        return -1;
    }

    //    external fun destory(player:Long): Unit
    fun destory(player:Long): Unit {

    }

//    external fun invoke(handle:Long,data: ByteArray): Int

    fun invoke(handle:Long,data: ByteArray): Int {
        return -1;
    }

//    external fun setSurface(handle:Long,it: Surface?)

    fun setSurface(handle:Long,it: Surface?) {

    }
    //    external fun setCallBack(handle:Long,callback:JustCallBack): Unit
    fun setCallBack(handle:Long,callback:JustCallBack): Unit {

    }

//    external fun testmain(name:ByteArray,desc:ByteArray):Unit

    fun testmain(name:ByteArray,desc:ByteArray):Unit {

    }

//    companion object {
//        init {
//            System.loadLibrary("JustApi")
//            System.loadLibrary("swscale")
//            System.loadLibrary("avformat")
//            System.loadLibrary("avdevice")
//            System.loadLibrary("avfilter")
//            System.loadLibrary("avutil")
//            System.loadLibrary("avcodec")
//            System.loadLibrary("swresample")
//            System.loadLibrary("JUSTPlayer")
//            System.loadLibrary("CPPTest")
//        }
//    }






}