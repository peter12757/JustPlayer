package com.eathemeat.justplayer.launcher.screen

import android.util.Log
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.tooling.preview.Preview
import androidx.lifecycle.viewmodel.compose.viewModel
import com.eathemeat.justplayer.launcher.MainViewModule
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme

/**
 * author:PeterX
 * time:2024/4/21 0021
 */
@Composable
fun MainContainScreen(modifier: Modifier = Modifier, viewModule: MainViewModule = viewModel()) {

}


@Preview(showBackground = true)
@Composable
fun MainContainScreenPreview() {
    JustPlayerTheme {
        MainContainScreen()
    }
}