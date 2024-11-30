package com.eathemeat.justplayer.data

import android.os.Environment
import android.util.Log
import androidx.compose.ui.graphics.Path
import java.io.File

class SDcardFileGetter {


    val defPath = Environment.getExternalStorageDirectory().path + "/Download/testvideo"

    val TAG = "SDcardFileGetter"
    var mPath = defPath

    fun scanFile(path: String): List<File> {
        Log.d(TAG, "scanFile() called with: path = $path")
        var file = File(path)
        if (!file.exists()) {
            return listOf()
        } else if (file.isFile) {
            return listOf()
        } else {
            var files = file.listFiles()
            return files.asList()
        }
    }

//    fun open(path: String): File {
//
//    }


    fun isSDcardValid(): Boolean {
        return Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)
    }


}