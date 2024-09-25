package com.eathemeat.player.ffmpeg

import java.nio.ByteBuffer

class FFMsgHandler{

    companion object {
        enum class HANDLE_URI{
            AUDIO,
        }

        @JvmStatic
        val handleMap = mutableMapOf<Int, IHandle>()

        @JvmStatic
        fun regist(handle: IHandle, replace:Boolean = false): Int {
            var key = handle.handleUri()
            handleMap[key]?.also {
                throw IllegalArgumentException("FFMsgHandler already has exist uri :${key}")
            }
            handleMap[key]?:also {
                handleMap.put(key,handle)
                return key
            }
            return -1;
        }

        @JvmStatic
        fun unRegister(handle: IHandle):Unit{
            handleMap[handle.handleUri()]?:also {
                handleMap.remove(handle.handleUri())
            }
        }
    }

    /**
     * be called by jni
     */
    fun onCallBack(inData: ByteArray): Any? {
        var data = ByteBuffer.wrap(inData)
        var handleUri = data.int
        var handle = handleMap[handleUri]
        var result:ByteArray? = null
        handle?.also {
            return it.handle(data)
        }
        return null
    }

}

abstract class IHandle {
    abstract fun handleUri():Int
    abstract fun handle(data: ByteBuffer): Any?
}
