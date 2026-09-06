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
public val PIcons.Play.PlayListIcon: ImageVector
    get() {
        if (_list != null) {
            return _list!!
        }
        _list =
            ImageVector.Builder(
                name = "list",
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
                        moveTo(14.55f, 16.15f)
                        verticalLineTo(11.4f)
                        horizontalLineTo(43.3f)
                        verticalLineToRelative(4.75f)
                        horizontalLineTo(14.55f)
                        close()
                        moveToRelative(0f, 10.2f)
                        verticalLineToRelative(-4.7f)
                        horizontalLineTo(43.3f)
                        verticalLineToRelative(4.7f)
                        horizontalLineTo(14.55f)
                        close()
                        moveToRelative(0f, 10.25f)
                        verticalLineTo(31.85f)
                        horizontalLineTo(43.3f)
                        verticalLineTo(36.6f)
                        horizontalLineTo(14.55f)
                        close()
                        moveTo(7.04f, 16.15f)
                        quadToRelative(-0.96f, 0f, -1.62f, -0.7f)
                        reflectiveQuadTo(4.75f, 13.75f)
                        quadToRelative(0f, -0.97f, 0.66f, -1.66f)
                        reflectiveQuadTo(7.06f, 11.4f)
                        reflectiveQuadToRelative(1.69f, 0.7f)
                        reflectiveQuadToRelative(0.7f, 1.72f)
                        quadToRelative(0f, 0.95f, -0.71f, 1.64f)
                        reflectiveQuadTo(7.04f, 16.15f)
                        close()
                        moveToRelative(0f, 10.2f)
                        quadToRelative(-0.96f, 0f, -1.62f, -0.7f)
                        reflectiveQuadTo(4.75f, 23.91f)
                        quadToRelative(0f, -0.96f, 0.66f, -1.61f)
                        reflectiveQuadTo(7.06f, 21.65f)
                        reflectiveQuadToRelative(1.69f, 0.66f)
                        reflectiveQuadToRelative(0.7f, 1.63f)
                        quadToRelative(0f, 1.01f, -0.71f, 1.71f)
                        reflectiveQuadToRelative(-1.71f, 0.7f)
                        close()
                        moveToRelative(0f, 10.2f)
                        quadToRelative(-0.96f, 0f, -1.62f, -0.7f)
                        reflectiveQuadTo(4.75f, 34.15f)
                        quadToRelative(0f, -0.97f, 0.66f, -1.66f)
                        reflectiveQuadTo(7.06f, 31.8f)
                        reflectiveQuadToRelative(1.69f, 0.7f)
                        reflectiveQuadToRelative(0.7f, 1.72f)
                        quadToRelative(0f, 0.95f, -0.71f, 1.64f)
                        reflectiveQuadTo(7.04f, 36.55f)
                        close()
                    }
                }
                .build()
        return _list!!
    }

@Preview(showBackground = true, widthDp = 100, heightDp = 100)
@Composable
fun PlayListIconPreview() {
    Icon(
        imageVector = PIcons.Play.PlayListIcon,
        contentDescription = "",
        modifier = Modifier.size(48.dp).background(Color.Black),
        tint = Color.Unspecified
    )
}
private var _list: ImageVector? = null
