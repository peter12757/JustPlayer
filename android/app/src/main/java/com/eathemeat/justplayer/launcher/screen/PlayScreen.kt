package com.eathemeat.justplayer.launcher.screen

import android.content.res.Configuration
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
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.compose.ui.viewinterop.AndroidView
import androidx.constraintlayout.compose.ConstraintLayout
import androidx.constraintlayout.compose.Dimension
import androidx.constraintlayout.compose.Visibility
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
    val config = LocalConfiguration.current
    if (config.orientation != Configuration.ORIENTATION_LANDSCAPE)
    {
        return
    }

    val showPlayList = remember {
        mutableStateOf(Visibility.Gone)
    }
    val showPlayControl = remember {
        mutableStateOf(Visibility.Visible)
    }

    ConstraintLayout {
        val context = LocalContext.current
        val (list, listBtn, control,surface) = createRefs()
        //surface
        AndroidView(modifier = Modifier.constrainAs(surface) {
            start.linkTo(parent.start)
            top.linkTo(parent.top)
            end.linkTo(parent.end)
            bottom.linkTo(parent.bottom)
            width = Dimension.fillToConstraints
        }.fillMaxSize(), factory = {
            SurfaceView(context).apply {
                // TODO:
            }
        })

        PlayListScreen(modifier = Modifier.constrainAs(list) {
            start.linkTo(parent.start)
            top.linkTo(parent.top)
            bottom.linkTo(parent.bottom)
            end.linkTo(listBtn.start)
            width = Dimension.ratio("8:10")
            visibility = showPlayList.value

        }, playItems = viewModule.playItems) {
            Log.d(TAG, "PlayScreen: ${it}")
            return@PlayListScreen 0
        }
        IconButton(onClick = {
            if (showPlayList.value == Visibility.Visible) showPlayList.value = Visibility.Gone
            else  showPlayList.value = Visibility.Visible },
            modifier = Modifier.constrainAs(listBtn) {
                start.linkTo(list.end)
                top.linkTo(parent.top, margin = 20.dp)
            }) {
            Icon(
                imageVector = when (showPlayList.value) {
                    Visibility.Visible -> Icons.Filled.KeyboardArrowLeft
                    else -> Icons.Filled.KeyboardArrowRight },
                contentDescription = when (showPlayList.value) {
                    Visibility.Visible -> stringResource(R.string.show_less)
                    else -> stringResource(R.string.show_more)}
            )
        }
        PlayControlScreen(modifier = Modifier.constrainAs(control) {
            start.linkTo(surface.start)
            end.linkTo(surface.end)
            bottom.linkTo(surface.bottom)
            width = Dimension.fillToConstraints
            height = Dimension.wrapContent
            visibility = showPlayControl.value
        })
    }
}

@Preview(showBackground = true, widthDp = 1080, heightDp = 720)
@Composable
fun PlayScreenPreview() {
    JustPlayerTheme {
        PlayScreen(viewModule = MainViewModule.test())
    }
}