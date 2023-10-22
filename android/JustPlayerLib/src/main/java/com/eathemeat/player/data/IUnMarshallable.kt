package com.eathemeat.player.data

import java.nio.ByteBuffer

interface IUnMarshallable {

    fun unmarshall(inBuf: ByteBuffer): Unit


}