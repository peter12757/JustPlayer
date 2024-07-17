package com.eathemeat.player.ffmpeg

import android.util.Log
import android.view.Surface
import com.eathemeat.base.IMediaPlayer
import com.eathemeat.base.MediaPlayerCallBack
import com.eathemeat.base.util.protocol.MarshallHelper
import java.net.URI

class FFPlayer(override var callback: MediaPlayerCallBack?) : IMediaPlayer {
    val TAG = FFPlayer::class.simpleName
    var nativeHandler = -1L


    init {
        nativeHandler = createPlayer("".encodeToByteArray())
    }

    override fun prepareAsyc() {
        Log.d(TAG, "prepareAsyc() called")
        checkNativeAvalible()
        var method = FFData.PrepareAsycMethod()
        invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())
    }

    override fun start() {
        Log.d(TAG, "start() called")
        checkNativeAvalible()
        var method = FFData.StartMethod()
        invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())
    }

    override fun pause() {
        Log.d(TAG, "pause() called")
        checkNativeAvalible()
        var method = FFData.PauseMethod()
        invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())
    }

    override fun stop() {
        Log.d(TAG, "stop() called")
        checkNativeAvalible()
        var method = FFData.StopMethod()
        invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())

    }

    override fun release() {
        Log.d(TAG, "release() called")
        checkNativeAvalible()
        destory(nativeHandler)
        nativeHandler = -1;
    }

    override fun reset() {
        Log.d(TAG, "reset() called")
        checkNativeAvalible()
        var method = FFData.ResetMethod()
        invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())

    }

    override fun setDataSource(uri: URI) {
        Log.d(TAG, "setDataSource() called with: uri = $uri")
        checkNativeAvalible()
        var method = FFData.SetDataSourceMethod(uri.toASCIIString())
        invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())

    }

    override fun setSurface(it: Surface?) {
        Log.d(TAG, "setSurface() called with: it = $it")
        setSurface(nativeHandler,it)
    }

    override fun seekTo(progress: Long, mode: Int) {
        Log.d(TAG, "seekTo() called with: progress = $progress, mode = $mode")
        checkNativeAvalible()
        // TODO: mode
        var method = FFData.SeekMethod(progress)
        invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())
    }

    override fun getDuration(): Long {
        Log.d(TAG, "getDuration() called")
        checkNativeAvalible()
        var method = FFData.GetDurationMethod()
        invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())
        // TODO:
        return 0
    }

    override fun getPosition(): Long {
        Log.d(TAG, "getPosition() called")
        checkNativeAvalible()
        var method = FFData.GetPositionMethod()
        invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())
        // TODO:
        return 0
    }

    fun checkNativeAvalible(): Unit {
//        assert(nativeHandler > 0)
    }



    /**
     * A native method that is implemented by the 'ffmpeglib' native library,
     * which is packaged with this application.
     */
    external fun createPlayer(context: ByteArray): Long


    external fun destory(player:Long): Unit


    external fun invoke(handle:Long,data: ByteArray): Int

    external fun setSurface(handle:Long,it: Surface?)

    external fun setCallBack(handle:Long,callback:FFMsgHandler): Unit

//    external fun testmain(name:ByteArray,desc:ByteArray):Unit
//
//    fun testmain(name:ByteArray,desc:ByteArray):Unit {
//
//    }

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