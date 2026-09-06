package com.eathemeat.justplayer.launcher.screen.play

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.fillMaxSize
import androidx.compose.foundation.layout.width
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.material3.TextField
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.text.style.TextOverflow
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.constraintlayout.compose.ConstraintLayout
import androidx.constraintlayout.compose.Dimension.Companion.fillToConstraints
import androidx.constraintlayout.compose.Dimension.Companion.wrapContent
import androidx.lifecycle.viewmodel.compose.viewModel
import com.eathemeat.justplayer.R
import com.eathemeat.justplayer.launcher.MainViewModel
import com.eathemeat.justplayer.launcher.widget.icons.PIcons
import com.eathemeat.justplayer.launcher.widget.icons.PlayListIcon
import com.eathemeat.justplayer.launcher.widget.icons.PlayMenuIcon
import com.eathemeat.justplayer.ui.theme.Black50
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme

@Composable
fun PlayTitleScreen(modifier: Modifier = Modifier, viewModule: MainViewModel = viewModel(), showPlayList:() -> Unit, showPlayMenu:()-> Unit) {
    ConstraintLayout(modifier = modifier.background(color = Black50).fillMaxSize()) {
        val (list, title, menu) = createRefs()
        val margindp = 10.dp
        IconButton(onClick =
            showPlayList,
            modifier = Modifier.constrainAs(list) {
                start.linkTo(parent.start,margin = margindp)
                end.linkTo(title.start)
                width = wrapContent
                centerVerticallyTo(parent)
            }) {
            Icon(
                imageVector = PIcons.Play.PlayListIcon,
                contentDescription = stringResource(R.string.show_playlist),
                modifier = modifier.fillMaxSize(),
                tint = Color.Unspecified
            )
        }
        Text(text = "unknown" ,style = MaterialTheme.typography.titleLarge, color = Color.White,
            maxLines = 1,
            overflow = TextOverflow.Ellipsis, // 超出部分显示省略号
            modifier = Modifier.constrainAs(title){
            start.linkTo(list.end, margin = margindp)
            end.linkTo(menu.start, margin = margindp)
            width =fillToConstraints
            centerVerticallyTo(parent)
        })
        IconButton(onClick =
            showPlayMenu,
            modifier = Modifier.constrainAs(menu) {
                start.linkTo(title.end)
                end.linkTo(parent.end, margin = margindp)
                width = wrapContent
                centerVerticallyTo(parent)
            }) {
            Icon(
                imageVector = PIcons.Play.PlayMenuIcon,
                modifier = modifier,
                contentDescription = stringResource(R.string.play_menu),
                tint = Color.Unspecified
            )
        }


    }

}


@Preview(showBackground = true, widthDp = 1080, heightDp = 72)
@Composable
fun PlayTitleScreenPreview() {
    JustPlayerTheme {
        PlayTitleScreen(modifier = Modifier, showPlayMenu = {}, showPlayList = { })
    }
}