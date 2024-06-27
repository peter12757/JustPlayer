package com.eathemeat.player.player.ffmpeg

import com.eathemeat.base.util.protocol.IMarshallable
import java.nio.ByteBuffer
import com.eathemeat.base.util.protocol.MarshallHelper


open class FFData {
    val  TAG = "JustData"


    enum class JUST_URI(val value:Int) {
        URI_UNKNOW(0),
        URI_METHOD(URI_UNKNOW.value+1),
    }

    enum class JUST_Method(val value:Int) {
        JUSTMETHOD_UNKNOW(0),
        JUSTMETHOD_PAUSE(JUSTMETHOD_UNKNOW.value+1),
        JUSTMETHOD_PREPARE(JUSTMETHOD_PAUSE.value+1),
        JUSTMETHOD_START(JUSTMETHOD_PREPARE.value+1),
        JUSTMETHOD_STOP(JUSTMETHOD_START.value+1),
        JUSTMETHOD_RESET(JUSTMETHOD_STOP.value+1),
        JUSTMETHOD_SET_DATA_SOURCE(JUSTMETHOD_RESET.value+1),
        JUSTMETHOD_SEEK(JUSTMETHOD_SET_DATA_SOURCE.value+1),
        JUSTMETHOD_GET_DURATION(JUSTMETHOD_SEEK.value+1),
        JUSTMETHOD_GET_POSITION(JUSTMETHOD_GET_DURATION.value+1),
//        METHOD_SET_DATA_SOURCE(METHOD_GET_POSITION.value+1),
    }


    open class MethodBase(var method:Int = JUST_Method.JUSTMETHOD_UNKNOW.value, var justUri:Int = JUST_URI.URI_METHOD.value) :
        IMarshallable {

        override fun size(): Int {
            return Int.SIZE_BYTES +Int.SIZE_BYTES
        }

        override fun marshal(out: ByteBuffer): ByteBuffer {
            out.putInt(size())
            out.putInt(justUri)
            out.putInt(method)
            return out
        }

    }

    class PauseMethod: MethodBase(JUST_Method.JUSTMETHOD_PAUSE.value)

    class PrepareAsycMethod: MethodBase(JUST_Method.JUSTMETHOD_PREPARE.value)

    class StartMethod: MethodBase(JUST_Method.JUSTMETHOD_START.value)

    class StopMethod: MethodBase(JUST_Method.JUSTMETHOD_STOP.value)

    class ResetMethod: MethodBase(JUST_Method.JUSTMETHOD_RESET.value)

    class SeekMethod(var prograss:Long): MethodBase(JUST_Method.JUSTMETHOD_SEEK.value){
        override fun size(): Int {
            return super.size()+Long.SIZE_BYTES
        }

        override fun marshal(out: ByteBuffer): ByteBuffer {
            return super.marshal(out)+prograss
        }

    }

    class GetDurationMethod(): MethodBase(JUST_Method.JUSTMETHOD_GET_DURATION.value)

    class GetPositionMethod(): MethodBase(JUST_Method.JUSTMETHOD_GET_POSITION.value)

    class SetDataSourceMethod(var source:String): MethodBase(JUST_Method.JUSTMETHOD_SET_DATA_SOURCE.value){
        override fun size(): Int {
            return super.size()+ MarshallHelper.calcMarshallSize(source)
        }

        override fun marshal(out: ByteBuffer): ByteBuffer {
            return super.marshal(out)+source
        }
    }


}





