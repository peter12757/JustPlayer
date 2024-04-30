package com.eathemeat.justplayer.launcher.screen.play

import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.twotone.PlayArrow
import androidx.compose.material3.FilledTonalIconButton
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.LinearProgressIndicator
import androidx.compose.material3.OutlinedIconButton
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
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
        IconButton(modifier = modifier.align(Alignment.CenterVertically).padding(0.dp)
            ,onClick = { /*TODO*/ }) {
            Icon(
                imageVector = PIcons.Play.PlayArrowLeft,
                contentDescription =
                stringResource(R.string.show_more))
        }
        IconButton(modifier = modifier.align(Alignment.CenterVertically)
            ,onClick = { /*TODO*/ }) {
            Icon(
                imageVector = Icons.TwoTone.PlayArrow,
                contentDescription =
                    stringResource(R.string.show_more))
        }
        IconButton(modifier = modifier.align(Alignment.CenterVertically)
            ,onClick = { /*TODO*/ }) {
            Icon(
                imageVector = PIcons.Play.PlayArrowRight,
                contentDescription =
                stringResource(R.string.show_more))
        }
        LinearProgressIndicator(progress = 0.5f,modifier = modifier.align(Alignment.CenterVertically))
        Text(modifier = modifier.align(Alignment.CenterVertically), maxLines = 1,text = "00:00:00/")
        Text(modifier = modifier.align(Alignment.CenterVertically), maxLines = 1,text = "00:00:00")
    }
}

@Preview(showBackground = true)
@Composable
fun PlayControlScreenPreview() {
    JustPlayerTheme {
        PlayControlScreen(viewModule = MainViewModule.test())
    }
}