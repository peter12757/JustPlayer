package com.eathemeat.player.player.sys

import android.media.MediaPlayer
import android.os.Build
import android.util.Log
import android.view.Surface
import com.eathemeat.base.IMediaPlayer
import com.eathemeat.base.MediaPlayerCallBack
import java.net.URI

class AndroidMediaPlayer(override var callback: MediaPlayerCallBack?) : IMediaPlayer {
    val TAG = AndroidMediaPlayer::class.simpleName

    var mediaPlayer = MediaPlayer().apply {
        setOnBufferingUpdateListener { mp, percent ->
            callback?.onBufferingUpdate(this as IMediaPlayer, percent)

        }
        setOnPreparedListener {
            callback?.onPrepared()
        }
        setOnCompletionListener {
            callback?.onPlayComplted()
        }
        setOnErrorListener { mp, what, extra ->
            callback?.onError(this as IMediaPlayer,what,extra)
            true
        }
        setOnInfoListener { mp, what, extra ->
            Log.d(TAG, "setOnInfoListener() called with: mp = $mp, what = $what, extra = $extra")
            true
        }
        setOnSeekCompleteListener {
            callback?.onSeekCompleted()
        }

        setOnTimedTextListener { mp, text ->
            Log.d(TAG, "setOnTimedTextListener() called with: mp = $mp, text = $text")
            callback?.onTimedText(this as IMediaPlayer, text)
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            setOnTimedMetaDataAvailableListener { mp, data ->
                Log.d(
                    TAG,
                    "setOnTimedMetaDataAvailableListener() called with: mp = $mp, data = $data"
                )
            }
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            setOnSubtitleDataListener { mp, data ->
                Log.d(TAG, "setOnSubtitleDataListener() called with: mp = $mp, data = $data")
            }
        }
        setOnVideoSizeChangedListener { mp, width, height ->
            callback?.onVideoSizeChanged(this as IMediaPlayer,width,height)
        }
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.P) {
            setOnMediaTimeDiscontinuityListener { mp, mts ->
                Log.d(
                    TAG,
                    "setOnMediaTimeDiscontinuityListener() called with: mp = $mp, mts = $mts"
                )
            }
        }
    }

    override fun prepareAsyc() {
        mediaPlayer.prepareAsync()
    }

    override fun start() {
        mediaPlayer.start()
    }

    override fun pause() {
        mediaPlayer.pause()
    }

    override fun stop() {
        mediaPlayer.stop()
    }

    override fun release() {
        mediaPlayer.release()
        callback = null;
    }

    override fun reset() {
        mediaPlayer.reset()
    }

    override fun setDataSource(uri: URI) {
        mediaPlayer.setDataSource(uri.toString())
    }

    override fun setSurface(surface: Surface?) {
        mediaPlayer.setSurface(surface)
    }

    override fun seekTo(progress: Long, mode: Int) {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            mediaPlayer.seekTo(progress,mode)
        }else {
            mediaPlayer.seekTo(progress.toInt())
        }
    }

    override fun getDuration(): Long {
        return mediaPlayer.duration.toLong()
    }

    override fun getPosition(): Long {
        return mediaPlayer.currentPosition.toLong()
    }

}