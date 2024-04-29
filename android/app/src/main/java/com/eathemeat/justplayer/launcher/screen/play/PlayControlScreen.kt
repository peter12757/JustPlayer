package com.eathemeat.justplayer.launcher.screen.play

import androidx.compose.foundation.layout.Row
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.twotone.PlayArrow
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.lifecycle.viewmodel.compose.viewModel
import com.eathemeat.justplayer.R
import com.eathemeat.justplayer.launcher.MainViewModule
import com.eathemeat.justplayer.launcher.widget.icons.PIcons
import com.eathemeat.justplayer.launcher.widget.icons.PlayArrowLeft
import com.eathemeat.justplayer.launcher.widget.icons.PlayArrowRight
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme

/**
 * author:PeterX
 * time:2024/4/27 0027
 */
@Composable
fun PlayControlScreen(modifier: Modifier = Modifier, viewModule: MainViewModule = viewModel()) {
    Row {
        IconButton(onClick = { /*TODO*/ }) {
            Icon(
                imageVector = PIcons.Play.PlayArrowLeft,
                contentDescription =
                stringResource(R.string.show_more))
        }
        IconButton(onClick = { /*TODO*/ }) {
            Icon(
                imageVector = Icons.TwoTone.PlayArrow,
                contentDescription =
                    stringResource(R.string.show_more))
        }
        IconButton(onClick = { /*TODO*/ }) {
            Icon(
                imageVector = PIcons.Play.PlayArrowRight,
                contentDescription =
                stringResource(R.string.show_more))
        }
    }
}

@Preview(showBackground = true)
@Composable
fun PlayControlScreenPreview() {
    JustPlayerTheme {
        PlayControlScreen(viewModule = MainViewModule.test())
    }
}