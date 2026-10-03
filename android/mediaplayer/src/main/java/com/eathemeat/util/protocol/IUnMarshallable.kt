package com.eathemeat.base.util.protocol

import java.nio.ByteBuffer

interface IUnMarshallable {

    fun unmarshal(inBuf: ByteBuffer): Unit


}