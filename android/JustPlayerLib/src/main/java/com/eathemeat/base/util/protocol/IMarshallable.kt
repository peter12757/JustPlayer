package com.eathemeat.base.util.protocol

import java.nio.ByteBuffer

interface IMarshallable {

    fun size(): Int

    fun marshal(outBuf: ByteBuffer): ByteBuffer

    operator fun ByteBuffer.plus(value: Long): ByteBuffer {
        putLong(value)
        return this
    }

    operator fun ByteBuffer.plus(source: String): ByteBuffer {
        MarshallHelper.marshall(this, source)
        return this
    }
}