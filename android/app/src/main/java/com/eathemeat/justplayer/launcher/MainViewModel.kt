package com.eathemeat.justplayer.launcher

import androidx.lifecycle.ViewModel
import com.eathemeat.justplayer.data.FileItem
import com.eathemeat.justplayer.data.Playitem
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

    val _playItems = MutableStateFlow(mutableListOf<Playitem>())
    val playItems: StateFlow<MutableList<Playitem>> = _playItems.asStateFlow()

    init {

    }

}