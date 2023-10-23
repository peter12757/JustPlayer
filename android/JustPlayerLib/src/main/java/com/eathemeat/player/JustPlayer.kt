package com.eathemeat.player

import android.util.Log
import android.view.Surface
import com.eathemeat.base.IMediaPlayer
import com.eathemeat.base.MediaPlayerCallBack
import com.eathemeat.player.data.MarshallHelper
import java.net.URI

class JustPlayer(override var callback: MediaPlayerCallBack?) : IMediaPlayer {
    val TAG = JustPlayer::class.simpleName
    var api:JustApi = JustApi()
    var nativeHandler = -1L


    init {
        nativeHandler = api.createPlayer()
    }

    override fun prepareAsyc() {
        Log.d(TAG, "prepareAsyc() called")
        checkNativeAvalible()
        var method = JustData.PrepareAsycMethod()
        api.invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())
    }

    override fun start() {
        Log.d(TAG, "start() called")
        checkNativeAvalible()
        var method = JustData.StartMethod()
        api.invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())
    }

    override fun pause() {
        Log.d(TAG, "pause() called")
        checkNativeAvalible()
        var method = JustData.PauseMethod()
        api.invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())
    }

    override fun stop() {
        Log.d(TAG, "stop() called")
        checkNativeAvalible()
        var method = JustData.StopMethod()
        api.invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())

    }

    override fun release() {
        if(nativeHandler >0)
        api.destory(nativeHandler)
        nativeHandler = -1;
    }

    override fun reset() {
        Log.d(TAG, "reset() called")
        checkNativeAvalible()
        var method = JustData.ResetMethod()
        api.invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())

    }

    override fun setDataSource(uri: URI) {
        Log.d(TAG, "setDataSource() called with: uri = $uri")
        checkNativeAvalible()
        var method = JustData.SetDataSourceMethod(uri.toASCIIString())
        api.invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())

    }

    override fun setSurface(it: Surface?) {
        Log.d(TAG, "setSurface() called with: it = $it")
        api.setSurface(nativeHandler,it)
    }

    override fun seekTo(progress: Long, mode: Int) {
        Log.d(TAG, "seekTo() called with: progress = $progress, mode = $mode")
        checkNativeAvalible()
        // TODO: mode
        var method = JustData.SeekMethod(progress)
        api.invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())
    }

    override fun getDuration(): Long {
        Log.d(TAG, "getDuration() called")
        checkNativeAvalible()
        var method = JustData.GetDurationMethod()
        api.invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())
        // TODO:
        return 0
    }

    override fun getPosition(): Long {
        Log.d(TAG, "getPosition() called")
        checkNativeAvalible()
        var method = JustData.GetPositionMethod()
        api.invoke(nativeHandler, MarshallHelper.packageToByteBuffer(method).array())
        // TODO:
        return 0
    }

    fun checkNativeAvalible(): Unit {
//        assert(nativeHandler > 0)
    }
}