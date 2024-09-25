package com.eathemeat.justplayer.launcher.screen

import android.content.res.Configuration
import android.util.Log
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.platform.LocalConfiguration
import androidx.compose.ui.platform.LocalLifecycleOwner
import androidx.compose.ui.tooling.preview.Preview
import androidx.lifecycle.Lifecycle
import androidx.lifecycle.LifecycleEventObserver
import androidx.lifecycle.viewmodel.compose.viewModel
import com.eathemeat.justplayer.launcher.MainViewModel
import com.eathemeat.justplayer.launcher.TAG
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme


enum class SubScreen {
    Screen_Launcher,
    Screen_Play,
    Screen_Config
}
/**
 * author:PeterX
 * time:2024/4/21 0021
 */
@Composable
fun MainContainScreen(modifier: Modifier = Modifier, viewModule: MainViewModel = viewModel()) {
    var state = remember {
        mutableStateOf(SubScreen.Screen_Launcher)
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

    //state change？
    when(state.value){
        SubScreen.Screen_Launcher -> {
            val configuration = LocalConfiguration.current
            configuration.orientation = Configuration.ORIENTATION_PORTRAIT
            LauncherScreen {
                state.value = SubScreen.Screen_Play
            }
        }
        SubScreen.Screen_Play ->{
            val configuration = LocalConfiguration.current
            configuration.orientation = Configuration.ORIENTATION_LANDSCAPE
            PlayScreen()
        }
        SubScreen.Screen_Config ->{
            val configuration = LocalConfiguration.current
            configuration.orientation = Configuration.ORIENTATION_PORTRAIT
            ConfigScreen()
        }
        else ->{
            ErrorScreen(errMsg = "ErrorMsg")
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