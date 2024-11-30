package com.eathemeat.justplayer.launcher.widget.icons

import androidx.compose.material.icons.materialPath
import androidx.compose.ui.graphics.PathFillType
import androidx.compose.ui.graphics.vector.ImageVector
import androidx.compose.ui.unit.dp

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
            defaultWidth = 24.dp,
            defaultHeight = 24.dp,
            viewportWidth = 960f,
            viewportHeight = 960f
        ).apply {
            materialPath(fillAlpha = 1f, pathFillType =  PathFillType.NonZero) {
                moveTo(220f,720f)
                lineTo(220f,240f)
                lineTo(300f,240f)
                lineTo(300f,720f)
                lineTo(220f,720f)
                close()
                moveTo(740f,720f)
                lineTo(380f,480f)
                lineTo(740f,240f)
                lineTo(740f,720f)
                close()
                moveTo(660f,480f)
                lineTo(660f,480f)
                lineTo(660f,480f)
                lineTo(660f,480f)
                close()
                moveTo(660f,570f)
                lineTo(660f,390f)
                lineTo(524f,480f)
                lineTo(660f,570f)
                close()

            }
        }.build()
        return _playArrowLeft!!
    }



private var _playArrowLeft:ImageVector? = null