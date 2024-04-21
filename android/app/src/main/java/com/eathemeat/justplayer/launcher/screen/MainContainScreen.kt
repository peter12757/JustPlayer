package com.eathemeat.justplayer.launcher.screen

import android.util.Log
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.SideEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.rememberCoroutineScope
import androidx.compose.runtime.setValue
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalLifecycleOwner
import androidx.compose.ui.tooling.preview.Preview
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver
import androidx.lifecycle.viewmodel.compose.viewModel
import com.eathemeat.justplayer.launcher.MainViewModule
import com.eathemeat.justplayer.launcher.TAG
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme
import kotlinx.coroutines.delay
import kotlinx.coroutines.launch

/**
 * author:PeterX
 * time:2024/4/21 0021
 */
@Composable
fun MainContainScreen(modifier: Modifier = Modifier, viewModule: MainViewModule = viewModel()) {
    LauncherScreen {
        Log.d(TAG, "MainContainScreen: end")
    }


    val lifecycle = LocalLifecycleOwner.current.lifecycle
    DisposableEffect(key1 = lifecycle, key2 = viewModule) {
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
fun MainContainScreenPreview() {
    JustPlayerTheme {
        MainContainScreen()
    }
}