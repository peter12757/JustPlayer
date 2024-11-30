package com.eathemeat.justplayer.launcher.screen.play

import android.util.Log
import androidx.compose.animation.animateContentSize
import androidx.compose.animation.core.Spring
import androidx.compose.animation.core.spring
import androidx.compose.foundation.layout.Column
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.padding
import androidx.compose.foundation.lazy.LazyColumn
import androidx.compose.foundation.lazy.items
import androidx.compose.material.icons.Icons
import androidx.compose.material.icons.filled.KeyboardArrowDown
import androidx.compose.material.icons.filled.KeyboardArrowUp
import androidx.compose.material.icons.filled.PlayArrow
import androidx.compose.material3.Card
import androidx.compose.material3.CardDefaults
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember
import androidx.compose.ui.Modifier
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.text.font.FontWeight
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.Dp
import androidx.compose.ui.unit.dp
import com.eathemeat.justplayer.R
import com.eathemeat.justplayer.data.PlayItem
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme

const val TAG = "PlayListScreen"
/**
 * author:PeterX
 * time:2024/4/22 0022
 */
@Composable
fun PlayListScreen(
    modifier: Modifier = Modifier, playItems: List<PlayItem>,
    index:Int = -1,
    play: (item: PlayItem) -> Int
) {
    LazyColumn(modifier = modifier.padding(vertical = 4.dp)) {
        items(items = playItems) {
            PlayItemScreen(playItem = it, play = play)
        }
    }
}

@Composable
fun PlayItemScreen(modifier: Modifier = Modifier,playItem:PlayItem,play:(item:PlayItem)->Int) {
    Card(colors =  CardDefaults.cardColors(containerColor = MaterialTheme.colorScheme.primaryContainer)
    , modifier = modifier.padding(vertical = Dp(4f), horizontal = Dp(8f))
    ) {
        //card content
        var expanded = remember { mutableStateOf(true) }
        Row(
            modifier = Modifier
                .padding(12.dp)
                .animateContentSize(
                    animationSpec = spring(
                        dampingRatio = Spring.DampingRatioMediumBouncy,
                        stiffness = Spring.StiffnessLow
                    )
                )
        ) {
            Column(
                modifier = Modifier
                    .weight(1f)
                    .padding(12.dp)
            ) {
                Text(
                    text = playItem.name, style = MaterialTheme.typography.headlineMedium.copy(
                        fontWeight = FontWeight.ExtraBold
                    )
                )
                if (expanded.value) {
                    Text(
                        text = ("Composem ipsum color sit lazy, " +
                                "padding theme elit, sed do bouncy. ").repeat(4),
                    )
                }
            }
            IconButton(onClick = { play(playItem) }) {
                Icon(
                    imageVector = Icons.Filled.PlayArrow,
                    contentDescription = stringResource(R.string.play)
                )
            }
            IconButton(onClick = { expanded.value = !expanded.value }) {
                Icon(
                    imageVector = if (expanded.value) Icons.Filled.KeyboardArrowDown else Icons.Filled.KeyboardArrowUp,
                    contentDescription = if (expanded.value) {
                        stringResource(R.string.show_less)
                    } else {
                        stringResource(R.string.show_more)
                    }
                )
            }
        }
    }
}

val previewPlayList = mutableListOf<PlayItem>().apply {
    add(PlayItem("1","11111","123123123"))
    add(PlayItem("2","11111","123123123"))
    add(PlayItem("3","11111","123123123"))
    add(PlayItem("4","11111","123123123"))
    add(PlayItem("5","11111","123123123"))
    add(PlayItem("6","11111","123123123"))
    add(PlayItem("7","11111","123123123"))
    add(PlayItem("8","11111","123123123"))
    add(PlayItem("9","11111","123123123"))
}

@Preview(showBackground = true)
@Composable
fun PlaylistScreenPreview() {
    JustPlayerTheme {
        PlayListScreen(playItems = previewPlayList) {
            Log.d(TAG, "PlaylistScreenPreview: ${it}")
            return@PlayListScreen 0
        }
    }
}