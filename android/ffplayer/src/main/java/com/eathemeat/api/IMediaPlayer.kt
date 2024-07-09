package com.eathemeat.base

import android.media.MediaPlayer.SEEK_PREVIOUS_SYNC
import android.view.Surface
import java.net.URI

interface IMediaPlayer{

    var callback: MediaPlayerCallBack?
    fun prepareAsyc(): Unit

    fun start(): Unit

    fun pause(): Unit

    fun stop(): Unit

    fun release(): Unit

    fun reset():Unit

    fun setDataSource(uri:URI): Unit
    fun setSurface(it: Surface?)

    fun seekTo(progress: Long,mode:Int = SEEK_PREVIOUS_SYNC)

    fun getDuration(): Long

    fun getPosition():Long

}