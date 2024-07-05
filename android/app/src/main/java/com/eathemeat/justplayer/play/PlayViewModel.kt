package com.eathemeat.justplayer.play

import android.media.TimedText
import android.util.Log
import android.view.Surface
import android.view.SurfaceHolder
import androidx.annotation.Nullable
import androidx.lifecycle.LiveData
import androidx.lifecycle.MutableLiveData
import androidx.lifecycle.ViewModel
import com.eathemeat.base.IMediaPlayer
import com.eathemeat.base.MediaPlayerCallBack
import com.eathemeat.justplayer.data.PlayItem
import com.eathemeat.player.player.sys.AndroidMediaPlayer
import java.net.URI

class PlayViewModel: ViewModel(), MediaPlayerCallBack {

//livedata or stateflow
//    var



    private val TAG = "PlayViewModel"


    var mPlayList = MutableLiveData<PlayItem>()
    private var mPlayer: IMediaPlayer? = null
    //    private var mPlayer: IMediaPlayer = JustPlayer(JustData.JustContext(JustPlayerType.FFMPEGPlayer),this)
    private var surface:Surface? = null
    var mCurPlayItem:PlayItem? = null


    fun setSurface(surface: Surface?) {
        this.surface = surface
    }

    fun play(item:PlayItem?) {
        item?:let {
            stop()
            return@let
        }
        item?.let { item ->
            mCurPlayItem = item
            mPlayer?: let {
                createPlayer()
            }?.apply {
                setSurface(surface)
                setDataSource(URI.create(item.url))
                prepareAsyc()
            }
        }



    }

    fun stop() {
        mPlayer = mPlayer?.let { player->
            player.stop()
            player.release()
            null
        }
    }

    private fun createPlayer(): IMediaPlayer? {
        return mPlayer?.let {player->
            player.stop()
            player.release()
            AndroidMediaPlayer(this@PlayViewModel)
        }
    }

    override fun onPrepared() {
        Log.d(TAG, "onPrepared() called")
        mPlayer?.start()
    }

    override fun onBufferring() {
        Log.d(TAG, "onBufferring() called")

    }

    override fun onSeekCompleted() {
        Log.d(TAG, "onSeekCompleted() called")
    }

    override fun onPlayComplted() {
        Log.d(TAG, "onPlayComplted() called")
    }

    override fun onInfo(what: Int, extra: Int) {
        Log.d(TAG, "onInfo() called with: what = $what, extra = $extra")
    }

    override fun onBufferingUpdate(mp: IMediaPlayer, percent: Int) {
        Log.d(TAG, "onBufferingUpdate() called with: mp = $mp, percent = $percent")
    }

    override fun onError(mp: IMediaPlayer, what: Int, extra: Int) {
        Log.d(TAG, "onError() called with: mp = $mp, what = $what, extra = $extra")
    }

    override fun onTimedText(mp: IMediaPlayer, text: TimedText?) {
        Log.d(TAG, "onTimedText() called with: mp = $mp, text = $text")
    }

    override fun onVideoSizeChanged(iMediaPlayer: IMediaPlayer, width: Int, height: Int) {
        Log.d(
            TAG,
            "onVideoSizeChanged() called with: iMediaPlayer = $iMediaPlayer, width = $width, height = $height"
        )
    }

    override fun onProgressUpdate(mediaPlayer: IMediaPlayer, progress: Long) {
        Log.d(
            TAG,
            "onProgressUpdate() called with: mediaPlayer = $mediaPlayer, progress = $progress"
        )
    }
}