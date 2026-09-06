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

/**
 * author:PeterX
 * time:2024/4/28 0028
 */
@Suppress("CheckReturnValue")
public val PIcons.Play.PauseIcon: ImageVector
get() {
    if (_pause != null) {
        return _pause!!
    }
    _pause =
        ImageVector.Builder(
            name = "pause",
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
                    moveTo(27.15f, 40.3f)
                    verticalLineTo(7.7f)
                    horizontalLineToRelative(12.2f)
                    verticalLineTo(40.3f)
                    horizontalLineTo(27.15f)
                    close()
                    moveToRelative(-18.5f, 0f)
                    verticalLineTo(7.7f)
                    horizontalLineToRelative(12.2f)
                    verticalLineTo(40.3f)
                    horizontalLineTo(8.65f)
                    close()
                }
            }
            .build()
    return _pause!!
}

@Preview(showBackground = true, widthDp = 100, heightDp = 100)
@Composable
fun PauseIconPreview() {
    Icon(
        imageVector = PIcons.Play.PauseIcon,
        contentDescription = "",
        modifier = Modifier.size(48.dp).background(Color.Black),tint = Color.Unspecified
    )
}

private var _pause: ImageVector? = null