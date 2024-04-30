package com.eathemeat.justplayer.launcher.screen

import android.util.Log
import android.view.SurfaceView
import androidx.compose.animation.animateContentSize
import androidx.compose.animation.core.Spring
import androidx.compose.animation.core.spring
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.fillMaxWidth
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.KeyboardArrowLeft
import androidx.compose.material.icons.filled.KeyboardArrowRight
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.runtime.Composable
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.viewinterop.AndroidView
import androidx.lifecycle.viewmodel.compose.viewModel
import com.eathemeat.justplayer.R
import com.eathemeat.justplayer.launcher.MainViewModule
import com.eathemeat.justplayer.launcher.screen.play.PlayControlScreen
import com.eathemeat.justplayer.launcher.screen.play.PlayListScreen
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme

const val TAG = "PlayScreen"

/**
 * author:PeterX
 * time:2024/4/21 0021
 */
@Composable
fun PlayScreen(modifier: Modifier = Modifier, viewModule: MainViewModule = viewModel()) {
    val showPlayList = remember {
        mutableStateOf(false)
    }
    val showPlayControl = remember {
        mutableStateOf(true)
    }
    Box(modifier = modifier.fillMaxSize()) {
        val context = LocalContext.current
        //surface
        AndroidView(modifier = modifier.fillMaxSize()
            ,factory = {
            SurfaceView(context).apply {
                // TODO:  
            }
        })
        //show play list
        Row(modifier = modifier.animateContentSize (animationSpec = spring(
            dampingRatio = Spring.DampingRatioMediumBouncy,
            stiffness = Spring.StiffnessLow
        )
        )) {
            if (showPlayList.value) {
                PlayListScreen(modifier = modifier.fillMaxWidth(0.8f),playItems = viewModule.playItems) {
                    Log.d(TAG, "PlayScreen: ${it}")
                    return@PlayListScreen 0
                }
            }
            IconButton(onClick = { showPlayList.value = !showPlayList.value }) {
                Icon(
                    imageVector = if (showPlayList.value) Icons.Filled.KeyboardArrowLeft else Icons.Filled.KeyboardArrowRight,
                    contentDescription = if (showPlayList.value) {
                        stringResource(R.string.show_less)
                    } else {
                        stringResource(R.string.show_more)
                    }
                )
            }
            
        }
        if (showPlayControl.value) {
            PlayControlScreen(modifier = Modifier.align(Alignment.BottomStart))
        }




    }
}

@Preview(showBackground = true)
@Composable
fun PlayScreenPreview() {
    JustPlayerTheme {
        PlayScreen(viewModule = MainViewModule.test())
    }
}