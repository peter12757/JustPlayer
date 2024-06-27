package com.eathemeat.justplayer.play

import android.media.TimedText
import androidx.lifecycle.ViewModel
import com.eathemeat.base.IMediaPlayer
import com.eathemeat.base.MediaPlayerCallBack
import com.eathemeat.player.JustData
import com.eathemeat.player.JustData.JustPlayerType
import com.eathemeat.player.JustPlayer

class PlayViewModel: ViewModel(), MediaPlayerCallBack {

//livedata or stateflow
//    var

    private var mPlayer:JustPlayer = JustPlayer(JustData.JustContext(JustPlayerType.FFMPEGPlayer),this)




    override fun onPrepared() {
        TODO("Not yet implemented")
    }

    override fun onBufferring() {
        TODO("Not yet implemented")
    }

    override fun onSeekCompleted() {
        TODO("Not yet implemented")
    }

    override fun onPlayComplted() {
        TODO("Not yet implemented")
    }

    override fun onInfo(what: Int, extra: Int) {
        TODO("Not yet implemented")
    }

    override fun onBufferingUpdate(mp: IMediaPlayer, percent: Int) {
        TODO("Not yet implemented")
    }

    override fun onError(mp: IMediaPlayer, what: Int, extra: Int) {
        TODO("Not yet implemented")
    }

    override fun onTimedText(mp: IMediaPlayer, text: TimedText?) {
        TODO("Not yet implemented")
    }

    override fun onVideoSizeChanged(iMediaPlayer: IMediaPlayer, width: Int, height: Int) {
        TODO("Not yet implemented")
    }

    override fun onProgressUpdate(mediaPlayer: IMediaPlayer, progress: Long) {
        TODO("Not yet implemented")
    }

}