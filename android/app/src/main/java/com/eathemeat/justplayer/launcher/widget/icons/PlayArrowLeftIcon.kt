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
 * M142,730L142,230Q142,202 162,182Q182,162 210,162Q239,162 258.5,182Q278,202 278,230L278,730Q278,758 258.5,778Q239,798 210,798Q182,798 162,778Q142,758 142,730ZM712,727L425,536Q410,527 403,511.5Q396,496 396,480Q396,464 403,448.5Q410,433 425,424L712,233Q721,227 730.5,224.5Q740,222 750,222Q777,222 797.5,241Q818,260 818,290L818,670Q818,700 797.5,719Q777,738 750,738Q740,738 730.5,735.5Q721,733 712,727Z
 */
public val PIcons.Play.PlayArrowLeft: ImageVector
    get() {
        if (_playArrowLeft != null)
            return _playArrowLeft!!
        _playArrowLeft = ImageVector.Builder(
            name = "Play.PlayArrowLeft",
            defaultWidth = 48.dp,
            defaultHeight = 48.dp,
            viewportWidth = 48f,
            viewportHeight = 48f,
        ).apply {
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
            )
            {
                moveTo(33.3f, 40.45f)
                lineTo(7.65f, 24.15f)
                lineTo(33.3f, 7.85f)
                verticalLineToRelative(32.6f)
                close()

            }
        }.build()
        return _playArrowLeft!!
    }

@Preview(showBackground = true, widthDp = 100, heightDp = 100)
@Composable
fun PlayArrowLeftPreview() {
    Icon(
        imageVector = PIcons.Play.PlayArrowLeft,
        contentDescription = "",
        tint = Color.Unspecified,
        modifier = Modifier.size(48.dp).background(Black50)
    )
}

private var _playArrowLeft:ImageVector? = null