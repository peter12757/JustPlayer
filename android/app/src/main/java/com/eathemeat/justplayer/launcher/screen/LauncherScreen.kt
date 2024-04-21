package com.eathemeat.justplayer.launcher.screen

import android.util.Log
import androidx.compose.foundation.Image
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.SideEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.platform.LocalLifecycleOwner
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.constraintlayout.compose.ConstraintLayout
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver
import androidx.lifecycle.viewmodel.compose.viewModel
import com.eathemeat.justplayer.R
import com.eathemeat.justplayer.launcher.MainViewModule
import com.eathemeat.justplayer.launcher.TAG
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch

/**
 * author:PeterX
 * time:2024/4/19 0019
 */
@Composable
fun LauncherScreen(modifier: Modifier = Modifier,viewModule: MainViewModule = viewModel()
,end:()-> Unit) {
    var clickTimes by remember {
        mutableIntStateOf(5)
    }
    Log.d(TAG, "Greeting: step1 clickTimes:${clickTimes}")

    SideEffect {
        Log.d(TAG, "Greeting: SideEffect here!!!")
        clickTimes--
    }
    val scope = rememberCoroutineScope()
    scope.launch {

        clickTimes--
        if (clickTimes <0 ) {
            end()
            return@launch
        }
        delay(1000L)

    }

    //sedounds
    ConstraintLayout {
        Box {
            Image(
                painterResource(id = R.drawable.img_def_launcher)
                ,modifier = modifier.fillMaxSize()
                , contentDescription = "default launcher image"
                , contentScale = ContentScale.FillBounds)
            Text(text = "${clickTimes}S",
                Modifier
                    .padding(all = 10.dp)
                    .align(Alignment.TopEnd)
                ,color = Color.White
            )
        }
    }
    val lifecycle = LocalLifecycleOwner.current.lifecycle
    DisposableEffect(key1 = lifecycle, key2 = clickTimes) {
        Log.d(TAG, "DisposableEffect:  step1")
        val lifeObserver = createLifeObserver()
        lifecycle.addObserver(lifeObserver)
        onDispose {
            Log.d(TAG, "DisposableEffect:  step1")
            lifecycle.removeObserver(lifeObserver)
        }
    }
}

fun createLifeObserver( ): LifecycleEventObserver =
    LifecycleEventObserver{ _,event ->
        when(event) {
            Lifecycle.Event.ON_CREATE ->{
                Log.d(TAG, "LifecycleEventObserver:  ON_CREATE")
            }
            Lifecycle.Event.ON_DESTROY ->{
                Log.d(TAG, "LifecycleEventObserver:  ON_DESTROY")
            }
            Lifecycle.Event.ON_RESUME ->{
                Log.d(TAG, "LifecycleEventObserver:  ON_RESUME")
            }
            Lifecycle.Event.ON_PAUSE ->{
                Log.d(TAG, "LifecycleEventObserver:  ON_PAUSE")
            }
            Lifecycle.Event.ON_START ->{
                Log.d(TAG, "LifecycleEventObserver:  ON_START")
            }
            Lifecycle.Event.ON_STOP ->{
                Log.d(TAG, "LifecycleEventObserver:  ON_STOP")
            }
            else -> {
                Log.d(TAG, "LifecycleEventObserver:  $event")
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




