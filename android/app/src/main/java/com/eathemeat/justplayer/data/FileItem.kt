package com.eathemeat.justplayer.data

import java.io.File
import java.net.URL

/**
 * author: PeterX
 * time: 2026/10/13 8:26
 */
data class FileItem(
    val name: String,
    val isDirectory: Boolean,
    val path: String,
    val size: Long,
    val file: File
)

fun FileItem.toPlayItem(): Playitem {
    return Playitem(name,"file://$path,size:$size",file.toURI().toURL())
}