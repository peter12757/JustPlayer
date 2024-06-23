package com.eathemeat.justplayer.launcher.screen

import android.content.Intent
import android.util.Log
import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.SideEffect
import androidx.compose.runtime.currentCompositionLocalContext
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.platform.LocalContext
import androidx.compose.ui.platform.LocalLifecycleOwner
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.constraintlayout.compose.ConstraintLayout
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver
import androidx.lifecycle.viewmodel.compose.viewModel
import com.eathemeat.justplayer.R
import com.eathemeat.justplayer.launcher.LauncherActivity
import com.eathemeat.justplayer.launcher.MainViewModule
import com.eathemeat.justplayer.launcher.TAG
import com.eathemeat.justplayer.play.PlayActivity
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme
import kotlinx.coroutines.delay
import kotlinx.coroutines.flow.catch
import kotlinx.coroutines.flow.flow
import kotlinx.coroutines.flow.onCompletion
import kotlinx.coroutines.flow.onStart
import kotlinx.coroutines.launch


/**
 * author:PeterX
 * time:2024/4/19 0019
 */
@Composable
fun LauncherScreen(modifier: Modifier = Modifier,viewModule: MainViewModule = viewModel()
,time:Int =5,end:()-> Unit) {
    val scope = rememberCoroutineScope()
    var clickTimes = remember{
        mutableIntStateOf(time)
    }
    val ctx = LocalContext.current
    scope.launch {
        flow {
            (clickTimes.intValue downTo 0).forEach {
                delay(1000L)
                emit(it)
            }
        }.onStart {
            Log.d(TAG, "LauncherScreen: onStart${clickTimes.intValue}")
        }.onCompletion {
            end()
            val intent = Intent(ctx,PlayActivity::class.java)
            ctx.startActivity(intent)
        }.catch {
            Log.e(TAG, "LauncherScreen: error", it)
        }.collect {
            clickTimes.value = it
        }
    }

    //sedounds
    ConstraintLayout {
        Box {
            Image(
                painterResource(id = R.drawable.img_def_launcher)
                ,modifier = modifier.fillMaxSize()
                , contentDescription = "default launcher image"
                , contentScale = ContentScale.FillBounds)
            Text(text = "${clickTimes.value} S",
                Modifier
                    .padding(all = 10.dp)
                    .align(Alignment.TopEnd)
                ,color = Color.White
            )
        }
    }

}


@Preview(showBackground = true)
@Composable
fun LauncherScreenPreview() {
    JustPlayerTheme {
        LauncherScreen(end = { Log.d(TAG, "preview: end")})
    }
}




