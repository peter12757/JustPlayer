package com.eathemeat.justplayer.launcher.screen.play

import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.lifecycle.viewmodel.compose.viewModel
import com.eathemeat.justplayer.launcher.MainViewModel
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme


/**
 * author: PeterX
 * time: 2026/9/6 8:31
 */
@Composable
fun PlayMenuScreen(modifier: Modifier = Modifier, viewModule: MainViewModel = viewModel()) {
    val TAG = "PlayMenuScreen"




}

@Preview(showBackground = true, widthDp = 1080, heightDp = 720)
@Composable
fun PlayMenuScreenPreview() {
    JustPlayerTheme {
        PlayMenuScreen()
    }
}