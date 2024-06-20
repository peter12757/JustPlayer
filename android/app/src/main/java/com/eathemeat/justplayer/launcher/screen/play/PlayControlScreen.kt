package com.eathemeat.justplayer.launcher.screen.play

import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.layout.width
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
import androidx.constraintlayout.compose.ConstraintLayout
import androidx.constraintlayout.compose.Dimension.Companion.fillToConstraints
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
    ConstraintLayout {
        val (preBtn,pauseBtn,nextBtn,progressBar,timeTxt) = createRefs()
        IconButton(modifier = Modifier.constrainAs(preBtn) {
            start.linkTo(parent.start, margin = 2.dp)
            centerVerticallyTo(parent)
        }
            ,onClick = { /*TODO*/ }) {
            Icon(
                imageVector = PIcons.Play.PlayArrowLeft,
                contentDescription =
                stringResource(R.string.show_more))
        }
        IconButton(modifier = Modifier.constrainAs(pauseBtn) {
            start.linkTo(preBtn.end)
            centerVerticallyTo(parent)
        }
            ,onClick = { /*TODO*/ }) {
            Icon(
                imageVector = Icons.TwoTone.PlayArrow,
                contentDescription =
                    stringResource(R.string.show_more))
        }
        IconButton(modifier = Modifier.constrainAs(nextBtn) {
            start.linkTo(pauseBtn.end)
            centerVerticallyTo(parent)
        }
            ,onClick = { /*TODO*/ }) {
            Icon(
                imageVector = PIcons.Play.PlayArrowRight,
                contentDescription =
                stringResource(R.string.show_more))
        }
        LinearProgressIndicator(progress = 0.5f,modifier = Modifier.constrainAs(progressBar) {
            start.linkTo(nextBtn.end)
            end.linkTo(timeTxt.start)
            width =fillToConstraints
            centerVerticallyTo(parent)
        })
        Text(modifier = Modifier.constrainAs(timeTxt) {
            start.linkTo(progressBar.end)
            end.linkTo(parent.end, margin = 2.dp)
            centerVerticallyTo(parent)
        }, maxLines = 1,text = "00:00:00/00:00:00")
    }
}

@Preview(showBackground = true, widthDp = 1080, heightDp = 720)
@Composable
fun PlayControlScreenPreview() {
    JustPlayerTheme {
        PlayControlScreen(viewModule = MainViewModule.test())
    }
}