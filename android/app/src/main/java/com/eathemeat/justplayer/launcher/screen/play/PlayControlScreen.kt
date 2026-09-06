package com.eathemeat.justplayer.launcher.screen.play

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.twotone.PlayArrow
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.LinearProgressIndicator
import androidx.compose.material3.ProgressIndicatorDefaults
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.constraintlayout.compose.ConstraintLayout
import androidx.constraintlayout.compose.Dimension.Companion.fillToConstraints
import androidx.lifecycle.viewmodel.compose.viewModel
import com.eathemeat.justplayer.R
import com.eathemeat.justplayer.launcher.MainViewModel
import com.eathemeat.justplayer.launcher.widget.icons.PIcons
import com.eathemeat.justplayer.launcher.widget.icons.PauseIcon
import com.eathemeat.justplayer.launcher.widget.icons.PlayArrowLeft
import com.eathemeat.justplayer.launcher.widget.icons.PlayArrowRight
import com.eathemeat.justplayer.ui.theme.Black50
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme

/**
 * author:PeterX
 * time:2024/4/27 0027
 */
@Composable
fun PlayControlScreen(modifier: Modifier = Modifier, viewModule: MainViewModel = viewModel()) {
    ConstraintLayout(modifier = modifier.background(Black50).fillMaxSize()) {
        val (preBtn,pauseBtn,nextBtn,progressBar,timeTxt) = createRefs()
        val marginDP = 5.dp
        IconButton(modifier = Modifier.constrainAs(preBtn) {
            start.linkTo(parent.start, margin = marginDP)
            end.linkTo(pauseBtn.start)
            centerVerticallyTo(parent)
        }
            ,onClick = { /*TODO*/ }) {
            Icon(
                imageVector = PIcons.Play.PlayArrowLeft,
                contentDescription =
                stringResource(R.string.play_pre),
                tint = Color.Unspecified,
            )
        }
        IconButton(modifier = Modifier.constrainAs(pauseBtn) {
            start.linkTo(preBtn.end, margin = marginDP)
            end.linkTo(nextBtn.start)
            centerVerticallyTo(parent)
        }
            ,onClick = { /*TODO*/ }) {
            Icon(
                imageVector = PIcons.Play.PauseIcon,
                tint = Color.Unspecified,
                contentDescription =
                    stringResource(R.string.play_pause))
        }
        IconButton(modifier = Modifier.constrainAs(nextBtn) {
            start.linkTo(pauseBtn.end, margin = marginDP)
            end.linkTo(progressBar.start)
            centerVerticallyTo(parent)
        }
            ,onClick = { /*TODO*/ }) {
            Icon(
                imageVector = PIcons.Play.PlayArrowRight,
                tint = Color.Unspecified,
                contentDescription =
                stringResource(R.string.play_next))

        }
        LinearProgressIndicator(
        progress = { 0.5f },
        modifier = Modifier.constrainAs(progressBar) {
                    start.linkTo(nextBtn.end, margin = marginDP)
                    end.linkTo(timeTxt.start)
                    width =fillToConstraints
                    centerVerticallyTo(parent)
                },
        color = ProgressIndicatorDefaults.linearColor,
        trackColor = ProgressIndicatorDefaults.linearTrackColor,
        strokeCap = ProgressIndicatorDefaults.LinearStrokeCap,
        )
        Text(modifier = Modifier.constrainAs(timeTxt) {
            start.linkTo(progressBar.end, margin = marginDP)
            end.linkTo(parent.end, margin = marginDP)
            centerVerticallyTo(parent)
        }, maxLines = 1,text = "00:00:00/00:00:00", color = Color.White)
    }
}

@Preview(showBackground = true, widthDp = 1080, heightDp = 72)
@Composable
fun PlayControlScreenPreview() {
    JustPlayerTheme {
        PlayControlScreen(modifier = Modifier, viewModule = MainViewModel.test())
    }
}