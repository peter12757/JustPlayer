package com.eathemeat.justplayer.launcher

import androidx.lifecycle.ViewModel
import com.eathemeat.justplayer.data.PlayItem
import com.eathemeat.justplayer.launcher.screen.play.previewPlayList
import com.eathemeat.justplayer.util.DirScanner
import kotlinx.coroutines.flow.MutableStateFlow
import kotlinx.coroutines.flow.StateFlow
import kotlinx.coroutines.flow.asStateFlow

/**
 * author:PeterX
 * time:2024/4/19 0019
 */
class MainViewModel : ViewModel() {

    private val image_url = "https://gitee.com/xipeitao/Note/blob/master/%E6%8A%80%E6%9C%AF/android/compose/res/demo.png"

    var launcherTime = 5000L

    val _playItems = MutableStateFlow(mutableListOf<DirScanner.FileItem>())
    val playItems: MutableList<DirScanner.FileItem> = _playItems.asStateFlow()


    companion object {
        fun test(): MainViewModel {
            return MainViewModel().apply {
                playItems.addAll(previewPlayList)
            }
        }
    }

}