package com.eathemeat.justplayer.launcher.screen.play

import androidx.compose.foundation.background
import androidx.compose.material3.Icon
import androidx.compose.material3.IconButton
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.res.stringResource
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import androidx.constraintlayout.compose.ConstraintLayout
import androidx.lifecycle.viewmodel.compose.viewModel
import com.eathemeat.justplayer.R
import com.eathemeat.justplayer.launcher.MainViewModel
import com.eathemeat.justplayer.launcher.widget.icons.PIcons
import com.eathemeat.justplayer.launcher.widget.icons.PlayListIcon
import com.eathemeat.justplayer.ui.theme.JustPlayerTheme

@Composable
fun PlayTitleScreen(modifier: Modifier = Modifier, viewModule: MainViewModel = viewModel(), showPlayList:() -> Unit, showPlayMenu:()-> Unit) {
    ConstraintLayout(modifier = modifier.background(color = Color.Black)) {
        val (list, title, menu) = createRefs()
        IconButton(onClick =
            showPlayList,
            modifier = Modifier.background(color = Color.White).constrainAs(list) {
                start.linkTo(parent.start, margin = 2.dp)
                top.linkTo(parent.top)
                centerVerticallyTo(parent)
            }) {
            Icon(
                imageVector = PIcons.Play.PlayListIcon,
                contentDescription = stringResource(R.string.show_playlist)
            )
        }


    }

}


@Preview(showBackground = true, widthDp = 1080, heightDp = 720)
@Composable
fun PlayTitleScreenPreview() {
    JustPlayerTheme {
        PlayTitleScreen(showPlayMenu = {}, showPlayList = { })
    }
}