package com.eathemeat.justplayer.launcher.screen

import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.lifecycle.viewmodel.compose.viewModel
import com.eathemeat.justplayer.launcher.MainViewModel
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme

/**
 * author:PeterX
 * time:2024/4/22 0022
 */
@Composable
fun ConfigScreen(modifier: Modifier = Modifier, viewModule: MainViewModel = viewModel()) {

}

@Preview(showBackground = true)
@Composable
fun ConfigScreenPreview() {
    JustPlayerTheme {
        PlayScreen()
    }
}