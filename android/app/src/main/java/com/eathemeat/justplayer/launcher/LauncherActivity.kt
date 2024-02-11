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
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.runtime.remember
import androidx.compose.runtime.setValue
import androidx.compose.ui.Alignment.Companion.TopEnd
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.layout.ContentScale
import androidx.compose.ui.res.painterResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.constraintlayout.compose.ConstraintLayout
import com.eathemeat.justplayer.R
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme

class LauncherActivity : ComponentActivity() {

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
}

private val timeColorArr = arrayOf(Color.Red,Color.Black,Color.Blue, Color.Gray,Color.Yellow,Color.White)
const val TAG = "LauncherActivity"

@Composable
fun Greeting(modifier: Modifier = Modifier) {

    var curColor = timeColorArr.random()
    var clickTimes by remember {
        mutableIntStateOf(0)
    }
    Log.d(TAG, "Greeting: step1 clickTimes:${clickTimes} curColor:${curColor}")

    //sedounds
    ConstraintLayout {
        Box {
            Image(painterResource(id = R.drawable.img_def_launcher)
                ,modifier = modifier
                , contentDescription = "default launcher image"
            , contentScale = ContentScale.FillBounds)
            Text(text = "${clickTimes}S",
                Modifier
                    .padding(all = 10.dp)
                    .align(TopEnd)
                    .clickable {
                        clickTimes++
                    }
                ,color = curColor
            )
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