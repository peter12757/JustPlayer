package com.eathemeat.justplayer.play

import android.icu.text.Transliterator.Position
import android.media.TimedText
import android.os.HandlerThread
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
import com.eathemeat.justplayer.data.SDcardFileGetter
import com.eathemeat.player.ffmpeg.FFPlayer
import com.eathemeat.player.sys.AndroidMediaPlayer
import com.eathemeat.util.media.PositionGetter
import java.net.URI

class PlayViewModel: ViewModel(), MediaPlayerCallBack, PositionGetter.OnPositionChangedListener {

//livedata or stateflow
    private val TAG = "PlayViewModel"


    var mPlayList = MutableLiveData<List<PlayItem>>()
//    private var mPlayer: IMediaPlayer = FFPlayer(this)      //default player
    private var mPlayer: IMediaPlayer = AndroidMediaPlayer(this)      //default player

    private var surface:Surface? = null
    var mCurPlayItem:PlayItem? = null
    val fileGetter = SDcardFileGetter()
    val videoSize = MutableLiveData<Pair<Int,Int>>()


    var posGetter: PositionGetter = PositionGetter(player = mPlayer, listener = this@PlayViewModel)
    val mPos = MutableLiveData<Long>()
    val mDuration = MutableLiveData<Long>()


    /**
     *  fffplayer & sysplayer should be switch in config
     *  not implement
     */
    fun switchPlayer() {
        NotImplementedError()
    }

    fun setSurface(surface: Surface?) {
        Log.d(TAG, "setSurface() called with: surface = $surface")
        surface?.run {
            this@PlayViewModel.surface = this
            mPlayer.setSurface(this)
        }
        surface?:run {
            stop()
        }
    }

    fun play(item:PlayItem?) {
        Log.d(TAG, "play() called with: item = $item")
        stop()
        item?.let { it ->
            mCurPlayItem = it
            mPlayer.apply {
                setSurface(surface)
                setDataSource(URI.create(it.url))
                prepareAsyc()
            }
        }
    }

    fun seekTo(position: Long): Unit {
        Log.d(TAG, "seekTo() called with: position = $position")
        mPlayer.seekTo(position)
    }

    fun stop() {
        Log.d(TAG, "stop() called")
        mPlayer.stop()
    }

    override fun onPrepared() {
        Log.d(TAG, "onPrepared() called")
        mPlayer.let{ player ->
            mDuration.value = player.getDuration()
            player.start()
            posGetter?.start()
        }
    }

    override fun onBufferring() {
        Log.d(TAG, "onBufferring() called")

    }

    override fun onSeekCompleted() {
        Log.d(TAG, "onSeekCompleted() called")
    }

    override fun onPlayComplted() {
        Log.d(TAG, "onPlayComplted() called")
        posGetter?.stop()
    }

    override fun onInfo(what: Int, extra: Int) {
        Log.d(TAG, "onInfo() called with: what = $what, extra = $extra")
    }

    override fun onBufferingUpdate(mp: IMediaPlayer, percent: Int) {
        Log.d(TAG, "onBufferingUpdate() called with: mp = $mp, percent = $percent")
    }

    override fun onError(mp: IMediaPlayer, what: Int, extra: Int) {
        Log.d(TAG, "onError() called with: mp = $mp, what = $what, extra = $extra")
        posGetter.stop()
        mPlayer.stop()
    }

    override fun onTimedText(iMediaPlayer: IMediaPlayer, text: TimedText?) {
        Log.d(TAG, "onTimedText() called with: iMediaPlayer = $iMediaPlayer, text = $text")
    }

    override fun onVideoSizeChanged(iMediaPlayer: IMediaPlayer, width: Int, height: Int) {
        Log.d(
            TAG,
            "onVideoSizeChanged() called with: iMediaPlayer = $iMediaPlayer, width = $width, height = $height"
        )
        videoSize.value = Pair(width,height)
        if (mDuration.value == 0L) {
            mDuration.value = mPlayer?.getDuration()
        }
    }

    override fun onProgressUpdate(mediaPlayer: IMediaPlayer, progress: Long) {
        Log.d(
            TAG,
            "onProgressUpdate() called with: mediaPlayer = $mediaPlayer, progress = $progress"
        )
    }

    fun scanfile() {
        var fileArr = fileGetter.scanFile(fileGetter.mPath)
        var list = mutableListOf<PlayItem>()
        fileArr.forEach { file->
            list.add(PlayItem(file.name,file.path,"${file.path}"))
        }
        mPlayList.value = list
    }

    override fun OnPositionChanged(mp: IMediaPlayer, pos: Long) {
        Log.d(TAG, "OnPositionChanged() called with: mp = $mp, pos = $pos")
        mPos.postValue(pos)
    }
}