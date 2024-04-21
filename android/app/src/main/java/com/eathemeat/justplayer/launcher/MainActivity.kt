@file:OptIn(ExperimentalMaterial3Api::class)

package com.eathemeat.justplayer.launcher

import android.os.Bundle
import androidx.activity.ComponentActivity
import androidx.activity.compose.setContent
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.material3.ExperimentalMaterial3Api
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Surface
import androidx.compose.runtime.Composable
import androidx.compose.runtime.mutableIntStateOf
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.tooling.preview.Preview
import com.eathemeat.justplayer.launcher.screen.LauncherScreen
import com.eathemeat.justplayer.launcher.screen.MainContainScreen
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
                    MainContainScreen ()
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

