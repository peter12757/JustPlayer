package com.eathemeat.justplayer.launcher.screen

import android.view.SurfaceView
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.Surface
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.viewinterop.AndroidView
import androidx.lifecycle.viewmodel.compose.viewModel
import com.eathemeat.justplayer.launcher.MainViewModule
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme

/**
 * author:PeterX
 * time:2024/4/21 0021
 */
@Composable
fun PlayScreen(modifier: Modifier = Modifier, viewModule: MainViewModule = viewModel()) {
    Box(modifier = modifier.fillMaxSize()) {
        val context = LocalContext.current
        //surface
        AndroidView(modifier = modifier.fillMaxSize()
            ,factory = {
            SurfaceView(context).apply {
                // TODO:  
            }
        })

    }
}

@Preview(showBackground = true)
@Composable
fun PlayScreenPreview() {
    JustPlayerTheme {
        PlayScreen()
    }
}