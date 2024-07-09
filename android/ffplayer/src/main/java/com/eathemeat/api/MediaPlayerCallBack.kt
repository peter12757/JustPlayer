package com.eathemeat.base

import android.media.TimedText

interface MediaPlayerCallBack {

    fun onPrepared(): Unit

    fun onBufferring(): Unit

    fun onSeekCompleted(): Unit

    fun onPlayComplted(): Unit

    fun onInfo(what: Int, extra: Int): Unit

    fun onBufferingUpdate(mp: IMediaPlayer, percent:Int):Unit

    fun onError(mp: IMediaPlayer, what: Int, extra:Int):Unit
    fun onTimedText(mp: IMediaPlayer, text: TimedText?)
    fun onVideoSizeChanged(iMediaPlayer: IMediaPlayer, width: Int, height: Int)
    fun onProgressUpdate(mediaPlayer: IMediaPlayer, progress: Long)




}