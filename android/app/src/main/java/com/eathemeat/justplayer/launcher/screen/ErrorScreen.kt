package com.eathemeat.justplayer.launcher.screen

import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.text.style.TextAlign
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.Dp
import androidx.lifecycle.viewmodel.compose.viewModel
import com.eathemeat.justplayer.launcher.MainViewModule
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme

/**
 * author:PeterX
 * time:2024/4/23 0023
 */

@Composable
fun ErrorScreen(modifier: Modifier = Modifier, errMsg:String) {
    Text(text = errMsg, textAlign = TextAlign.Center
        ,modifier = modifier.padding(top = Dp(140F)))
}

@Preview(showBackground = true)
@Composable
fun ErrorScreenPreview() {
    JustPlayerTheme {
        Surface(
            modifier = Modifier.fillMaxSize(),
            color = MaterialTheme.colorScheme.background
        ) {
            ErrorScreen(errMsg = "Error")
        }
    }
}