package com.eathemeat.justplayer.state

import android.os.Handler
import android.os.HandlerThread
import android.os.Message
import android.util.Log

class PLayThread : HandlerThread("PLayThread"), Handler.Callback {

    enum class State {
        IDLE,
        INITLIZAIALING,
        INITED,
        PREPAREING,
        PREPARED,
        STARTED,
        PAUSED,
        STOPPING,
        STOPPED,
        RELEASED,
    }
    lateinit var handler:Handler
    var mState = State.IDLE
    val TAG = "PLayThread"

    override fun start() {
        super.start()
        handler = Handler(looper,PLayThread@this)
    }

    override fun handleMessage(msg: Message): Boolean {
        Log.d(TAG, "handleMessage() called with: msg = $msg")
        var ret =false
        when(msg.what) {


        }
        return ret
    }

}