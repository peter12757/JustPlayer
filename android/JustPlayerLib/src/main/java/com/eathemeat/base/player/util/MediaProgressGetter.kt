package com.eathemeat.basedroid.player.util

import android.media.MediaPlayer
import android.os.Handler
import android.os.HandlerThread
import android.os.Looper
import android.os.Message
import android.util.Log

class MediaProgressGetter(name:String = "MediaTimeHandlerThread") : HandlerThread(name) {
    val TAG = MediaProgressGetter::class.java.name
    lateinit var handler: TimeHandler


    override fun start() {
        Log.d(TAG, "start() called")
        TODO("not impl")
    }

    fun startGet(player:MediaPlayer): Unit {
        player?.also {
            super.start()
            handler = TimeHandler(looper,it)
            handler.start()
        }
    }

    fun stopGet(): Unit {
        handler.stop()
        quitSafely()
    }

    fun setListener(listener: OnProgressUpdateListener?): Unit {
        handler?.also { it.setListener(listener) }
    }

    class TimeHandler(looper: Looper, val mp: MediaPlayer) : Handler(looper) {
        val MSG_SEDS = 0x01
        val TIME = 1000L //ms
        var mListener: OnProgressUpdateListener? = null

        override fun handleMessage(msg: Message) {
            when(msg.what){
                MSG_SEDS->{
                    Log.d(MediaProgressGetter::javaClass.name, "handleMessage() called with: msg = ${mp.currentPosition.toLong()}")
                    mListener?.onProgressUpdate(mp.currentPosition.toLong(),mp)
                    sendEmptyMessageDelayed(MSG_SEDS,TIME)
                }
            }
        }

        fun start() {
            sendEmptyMessage(MSG_SEDS)
        }

        fun stop(): Unit {
            removeCallbacksAndMessages(null)
        }

        fun setListener(listener: OnProgressUpdateListener?): Unit {
            mListener = listener
        }


    }

    interface OnProgressUpdateListener{
        fun onProgressUpdate(progress: Long, mediaPlayer: MediaPlayer): Unit
    }

}