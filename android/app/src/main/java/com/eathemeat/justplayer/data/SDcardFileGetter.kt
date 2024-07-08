package com.eathemeat.justplayer.data

import android.os.Environment

class SDcardFileGetter {


    val defPath = Environment.getExternalStorageDirectory()





    fun isSDcardValid() :Boolean {
        return Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)
    }

    
}