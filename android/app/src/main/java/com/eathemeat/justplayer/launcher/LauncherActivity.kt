@file:OptIn(ExperimentalMaterial3Api::class)

package com.eathemeat.justplayer.launcher

import android.os.Bundle
import android.util.Log
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.Image
import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Box
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.padding
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.DisposableEffect
import androidx.compose.runtime.SideEffect
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment.Companion.TopEnd
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
import com.eathemeat.justplayer.R
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme

class LauncherActivity : ComponentActivity() {
    
    private var wait_time = mutableIntStateOf(5)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContent {
            JustPlayerTheme {
                // A surface container using the 'background' color from the them e
                Surface(
                    modifier = Modifier.fillMaxSize(),
                    color = MaterialTheme.colorScheme.background
                ) {
                    Greeting()
                }
            }
        }
    }

    override fun onResume() {
        super.onResume()

    }
}

private val timeColorArr = arrayOf(Color.Red,Color.Black,Color.Blue, Color.Gray,Color.Yellow,Color.White)
const val TAG = "LauncherActivity"

private val image_url = "https://gitee.com/xipeitao/Note/blob/master/%E6%8A%80%E6%9C%AF/android/compose/res/demo.png"

@Composable
fun Greeting(modifier: Modifier = Modifier) {
    var clickTimes by remember {
        mutableIntStateOf(5)
    }
    Log.d(TAG, "Greeting: step1 clickTimes:${clickTimes}")

    SideEffect {
        Log.d(TAG, "Greeting: SideEffect here!!!")
    }

    //sedounds
    ConstraintLayout {
        Box {
            Image(painterResource(id = R.drawable.img_def_launcher)
                ,modifier = modifier.fillMaxSize()
                , contentDescription = "default launcher image"
            , contentScale = ContentScale.FillBounds)
            Text(text = "${clickTimes}S",
                Modifier
                    .padding(all = 10.dp)
                    .align(TopEnd)
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
fun GreetingPreview() {
    JustPlayerTheme {
        Greeting()
    }
}