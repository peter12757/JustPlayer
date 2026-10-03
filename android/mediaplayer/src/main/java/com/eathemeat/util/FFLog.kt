package com.eathemeat.util

import android.util.Log

object FFLog {


    fun d(TAG: String,msg: String,isTrace: Boolean =false){
        if (isTrace){
            Log.d(TAG, msg, Throwable())
        }else {
            Log.d(TAG, msg)
        }
    }

}