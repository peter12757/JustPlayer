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

@Suppress("CheckReturnValue")
public val PIcons.Play.PlayMenuIcon: ImageVector
    get() {
        if (_more_vert != null) {
            return _more_vert!!
        }
        _more_vert =
            ImageVector.Builder(
                name = "more_vert",
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
                        moveTo(24.03f, 41.7f)
                        quadToRelative(-1.23f, 0f, -2.11f, -0.87f)
                        reflectiveQuadTo(21.05f, 38.74f)
                        quadToRelative(0f, -1.17f, 0.87f, -2.08f)
                        reflectiveQuadToRelative(2.08f, -0.91f)
                        quadToRelative(1.26f, 0f, 2.11f, 0.91f)
                        reflectiveQuadToRelative(0.85f, 2.1f)
                        reflectiveQuadToRelative(-0.84f, 2.06f)
                        reflectiveQuadTo(24.03f, 41.7f)
                        close()
                        moveToRelative(0f, -14.75f)
                        quadToRelative(-1.23f, 0f, -2.11f, -0.87f)
                        reflectiveQuadTo(21.05f, 24.01f)
                        quadToRelative(0f, -1.26f, 0.87f, -2.11f)
                        reflectiveQuadToRelative(2.08f, -0.85f)
                        quadToRelative(1.26f, 0f, 2.11f, 0.84f)
                        reflectiveQuadToRelative(0.85f, 2.07f)
                        reflectiveQuadToRelative(-0.84f, 2.11f)
                        reflectiveQuadToRelative(-2.07f, 0.88f)
                        close()
                        moveToRelative(0f, -14.7f)
                        quadToRelative(-1.23f, 0f, -2.11f, -0.88f)
                        reflectiveQuadTo(21.05f, 9.24f)
                        reflectiveQuadTo(21.92f, 7.13f)
                        reflectiveQuadTo(23.99f, 6.25f)
                        quadToRelative(1.26f, 0f, 2.11f, 0.88f)
                        reflectiveQuadToRelative(0.85f, 2.12f)
                        reflectiveQuadToRelative(-0.84f, 2.12f)
                        reflectiveQuadToRelative(-2.07f, 0.88f)
                        close()
                    }
                }
                .build()
        return _more_vert!!
    }

@Preview(showBackground = true, widthDp = 100, heightDp = 100)
@Composable
fun PlayMenuIconPreview() {
    Icon(
        imageVector = PIcons.Play.PlayMenuIcon,
        contentDescription = "",
        modifier = Modifier.size(48.dp).background(Color.Black),tint = Color.Unspecified
    )
}

private var _more_vert: ImageVector? = null
