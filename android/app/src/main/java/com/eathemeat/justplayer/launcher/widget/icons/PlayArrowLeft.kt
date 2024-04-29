package com.eathemeat.justplayer.launcher.widget.icons

import androidx.compose.material.icons.materialIcon
import androidx.compose.material.icons.materialPath
import androidx.compose.ui.graphics.vector.ImageVector

/**
 * author:PeterX
 * time:2024/4/28 0028
 */
public val PIcons.Play.PlayArrowLeft: ImageVector
    get() {
        if (_playArrowLeft != null)
            return _playArrowLeft!!
        _playArrowLeft = materialIcon(name = "Play.PlayArrowLeft") {
            materialPath(fillAlpha = 0.3f, strokeAlpha = 0.3f) {
                moveTo(10.0f, 8.64f)
                verticalLineToRelative(6.72f)
                lineTo(15.27f, 12.0f)
                close()
            }
            materialPath {
                moveTo(8.0f, 19.0f)
                lineToRelative(11.0f, -7.0f)
                lineTo(8.0f, 5.0f)
                verticalLineToRelative(14.0f)
                close()
                moveTo(10.0f, 8.64f)
                lineTo(15.27f, 12.0f)
                lineTo(10.0f, 15.36f)
                lineTo(10.0f, 8.64f)
                close()
            }
        }
        return _playArrowLeft!!
    }



private var _playArrowLeft:ImageVector? = null