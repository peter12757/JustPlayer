package com.eathemeat.player.util.media

import android.os.Handler
import android.os.HandlerThread
import android.os.Message
import android.util.Log
import com.eathemeat.base.IMediaPlayer

class PositionGetter(
    val thread: HandlerThread = HandlerThread("def-position-getter").apply { start() },
    var player: IMediaPlayer,
    var listener:OnPositionChangedListener?
) : Handler(thread.looper) {
    val TAG = this::class.java.simpleName

    override fun handleMessage(msg: Message) {
        Log.d(TAG, "handleMessage() called with: msg = $msg")
        when (msg.what) {
            1 -> {
                listener?.let {
                    it.OnPositionChanged(player,player.getPosition())
                }
            }

            2 -> {

            }

            else -> {
                throw UnsupportedOperationException("msg what is not support")
            }
        }
    }

    fun start(): Unit {
        sendEmptyMessage(1)

    }

    fun stop(): Unit {
        sendEmptyMessage(2)
    }

    fun recycle() {
        thread.looper.quit()
    }

    interface OnPositionChangedListener{
        fun OnPositionChanged(mp:IMediaPlayer,pos:Long)
    }


}