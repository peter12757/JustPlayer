package com.eathemeat.justplayer.launcher.widget.icons

import androidx.compose.material.icons.materialIcon
import androidx.compose.material.icons.materialPath
import androidx.compose.ui.graphics.vector.ImageVector

/**
 * author:PeterX
 * time:2024/4/28 0028
 * M682,730L682,230Q682,202 701.5,182Q721,162 750,162Q778,162 798,182Q818,202 818,230L818,730Q818,758 798,778Q778,798 750,798Q721,798 701.5,778Q682,758 682,730ZM142,670L142,290Q142,260 162.5,241Q183,222 210,222Q220,222 229.5,224.5Q239,227 248,233L535,424Q550,433 557,448.5Q564,464 564,480Q564,496 557,511.5Q550,527 535,536L248,727Q239,733 229.5,735.5Q220,738 210,738Q183,738 162.5,719Q142,700 142,670Z
 */
public val PIcons.Play.PlayArrowRight: ImageVector
    get() {
        if (_playArrowRight != null)
            return _playArrowRight!!
        _playArrowRight = materialIcon(name = "Play.PlayArrowRight") {
            materialPath(fillAlpha = 1f) {
                moveTo(6f, 18f)
                lineToRelative(8.5f,-6f)
                lineTo(6f, 6f)
                moveTo(12f, 9f)
                verticalLineToRelative(12f)
                close()
                moveTo(16f,6f)
                verticalLineToRelative(12f)
                horizontalLineToRelative(2f)
                verticalLineTo(6f)
                horizontalLineToRelative(-2f)
                close()
            }
        }
        return _playArrowRight!!
    }



private var _playArrowRight: ImageVector? = null