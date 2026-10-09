package com.eathemeat.justplayer.util

import java.io.File

/**
 * author: PeterX
 * time: 2026/9/8 9:10
 */
object DirScanner {


    data class FileItem(
        val name: String,
        val isDirectory: Boolean,
        val path: String,
        val size: Long
    )

    fun listFiles(directory: File): List<FileItem> {
        if (!directory.exists() || !directory.isDirectory) return emptyList()

        return directory.listFiles()?.map { file ->
            FileItem(
                name = file.name,
                isDirectory = file.isDirectory,
                path = file.absolutePath,
                size = if (file.isFile) file.length() else 0L
            )
        } ?: emptyList()
    }








}