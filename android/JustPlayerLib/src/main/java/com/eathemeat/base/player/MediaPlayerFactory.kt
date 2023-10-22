package com.eathemeat.basedroid.player

import android.net.Uri
import com.alibaba.android.arouter.launcher.ARouter
import com.eathemeat.basedroid.data.test.Constants
import java.net.URI

class MediaPlayerFactory {

    companion object{
        fun create(path: String?): IMediaPlayer {
            ARouter.getInstance().inject(this)
            if (path == null) return AndroidMediaPlayer()
            var player = ARouter.getInstance().build(path!!).navigation()
            if (player == null) return AndroidMediaPlayer()
            return player as IMediaPlayer
        }
    }


}