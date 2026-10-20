package com.eathemeat.justplayer.util

import com.eathemeat.justplayer.data.FileItem
import java.io.File

/**
 * author: PeterX
 * time: 2026/9/8 9:10
 */
object DirScanner {
    fun listFiles(directory: File): List<FileItem> {
        if (!directory.exists() || !directory.isDirectory) return emptyList()

        return directory.listFiles()?.map { file ->
            FileItem(
                name = file.name,
                isDirectory = file.isDirectory,
                path = file.absolutePath,
                size = if (file.isFile) file.length() else 0L,
                file =file
            )
        } ?: emptyList()
    }

    fun listFiles(dirPath: String): List<FileItem> {
        var directory:File = File(dirPath)
        if (!directory.exists() || !directory.isDirectory) return emptyList()

        return directory.listFiles()?.map { file ->
            FileItem(
                name = file.name,
                isDirectory = file.isDirectory,
                path = file.absolutePath,
                size = if (file.isFile) file.length() else 0L,
                file =file
            )
        } ?: emptyList()
    }
}