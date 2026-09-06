package com.eathemeat.justplayer.launcher.widget.icons

import androidx.compose.foundation.background
import androidx.compose.foundation.layout.size
import androidx.compose.material3.Icon
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.compose.ui.graphics.Color
import androidx.compose.ui.graphics.PathFillType
import androidx.compose.ui.graphics.SolidColor
import androidx.compose.ui.graphics.StrokeCap
import androidx.compose.ui.graphics.StrokeJoin
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.graphics.vector.path
import androidx.compose.ui.tooling.preview.Preview
import androidx.compose.ui.unit.dp
import com.eathemeat.justplayer.ui.theme.Black50

/**
 * author:PeterX
 * time:2024/4/28 0028
 * M682,730L682,230Q682,202 701.5,182Q721,162 750,162Q778,162 798,182Q818,202 818,230L818,730Q818,758 798,778Q778,798 750,798Q721,798 701.5,778Q682,758 682,730ZM142,670L142,290Q142,260 162.5,241Q183,222 210,222Q220,222 229.5,224.5Q239,227 248,233L535,424Q550,433 557,448.5Q564,464 564,480Q564,496 557,511.5Q550,527 535,536L248,727Q239,733 229.5,735.5Q220,738 210,738Q183,738 162.5,719Q142,700 142,670Z
 */
public val PIcons.Play.PlayArrowRight: ImageVector
    get() {
        if (_play_arrow != null) {
            return _play_arrow!!
        }
        _play_arrow =
            ImageVector.Builder(
                name = "play_arrow",
                defaultWidth = 48.dp,
                defaultHeight = 48.dp,
                viewportWidth = 48f,
                viewportHeight = 48f,
            )
                .apply {
                    path(
                        fill = SolidColor(Color.White),
                        fillAlpha = 1f,
                        stroke = null,
                        strokeAlpha = 1f,
                        strokeLineWidth = 1f,
                        strokeLineCap = StrokeCap.Butt,
                        strokeLineJoin = StrokeJoin.Bevel,
                        strokeLineMiter = 1f,
                        pathFillType = PathFillType.Companion.NonZero,
                    ) {
                        moveTo(14.75f, 40.15f)
                        verticalLineTo(7.55f)
                        lineToRelative(25.6f, 16.3f)
                        lineToRelative(-25.6f, 16.3f)
                        close()
                    }
                }
                .build()
        return _play_arrow!!
    }

@Preview(showBackground = true, widthDp = 100, heightDp = 100)
@Composable
fun PlayArrowRightPreview() {
    Icon(
        imageVector = PIcons.Play.PlayArrowRight,
        contentDescription = "",
        tint = Color.Unspecified,
        modifier = Modifier.size(48.dp).background(Black50)
    )
}


private var _play_arrow: ImageVector? = null