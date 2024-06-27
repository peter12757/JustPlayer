package com.eathemeat.player.audio

import android.media.AudioAttributes
import android.media.AudioAttributes.*
import android.media.AudioFormat
import android.media.AudioFormat.*
import android.media.AudioManager.AUDIO_SESSION_ID_GENERATE
import android.media.AudioTrack
import com.eathemeat.player.player.ffmpeg.IHandle
import com.eathemeat.player.player.ffmpeg.JustCallBack
import com.eathemeat.base.util.protocol.IMarshallable
import com.eathemeat.base.util.protocol.MarshallHelper
import java.nio.ByteBuffer

class AudioHelper: IHandle() {

    companion object{
        val Method_GetMinBufferSize = 1
        val Method_CatchAll = Method_GetMinBufferSize+1
    }

    override fun handleUri(): Int {
        return JustCallBack.Companion.HANDLE_URI.AUDIO.ordinal
    }

    override fun handle(data: ByteBuffer):Any? {
        var method =data.int
        var result: IMarshallable? = when(method) {
            Method_GetMinBufferSize -> {
                var simpleRateInHz = data.int
                var channelConfig = data.int
                var audioFormat = data.int
                AudioGetMinBufferSize(AudioTrack.getMinBufferSize(simpleRateInHz,channelConfig,audioFormat))
            }
            Method_CatchAll ->{
                var streamType = data.int
                var sampleRateInHz = data.int
                var channelConfig = data.int
                var audioFormat = data.int
                var minBufferSize = data.int
                var mode = data.int
                //TODO AudioAttributes 还有其他还玩的接口和东西
                var attr = AudioAttributes.Builder().setContentType(CONTENT_TYPE_MUSIC)
                    .setFlags(FLAG_HW_AV_SYNC).setUsage(USAGE_MEDIA).build();
//                    .setAllowedCapturePolicy()PTURE_BY_NONE)
                var format = AudioFormat.Builder().setEncoding(ENCODING_PCM_16BIT).setSampleRate(sampleRateInHz).build()
//                    .setChannelMask().setChannelIndexMask(CHANNEL_OUT_DEFAULT)
                var obj =  AudioTrack(attr,format,minBufferSize,mode,AUDIO_SESSION_ID_GENERATE)
                return obj
            }

            else ->null
        }
        if (result == null) return ByteArray(0)
        else return MarshallHelper.packageToByteBuffer(result).array()

    }

    class AudioGetMinBufferSize(var minBufSize:Int,var uri:Int = Method_GetMinBufferSize):
        IMarshallable {

        override fun size(): Int {
            return Int.SIZE_BYTES+Int.SIZE_BYTES
        }

        override fun marshal(out: ByteBuffer): ByteBuffer {
            out.putInt(uri)
            out.putInt(minBufSize)
            return out
        }
    }

}