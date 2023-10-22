package com.eathemeat.player.data

import android.text.TextUtils
import android.util.Log
import com.eathemeat.player.JustData
import java.lang.Exception
import java.nio.BufferUnderflowException
import java.nio.ByteBuffer
import java.nio.ByteOrder


/**
 * 此协议不适用于大量数据
 */
class MarshallHelper {

    companion object {

        const val  TAG = "Packer"

        @JvmStatic
        fun marshall(bb: ByteBuffer, data: ByteArray?) {
            if (data != null) {
                bb.putShort(data.size.toShort())
                bb.put(data)
            } else {
                bb.putShort(0.toShort())
            }
        }

        /**
         * 序列化的时候需要注意如果是空字符串,unMarshall解析出来的结果是
         * null->"null"
         * ""->0
         * "abc"->"abc"
         *
         */
        @JvmStatic
        fun marshall(bb: ByteBuffer, string: String?) {
            if (string == null) {
                val data = "null".toByteArray()
                bb.putShort(data.size.toShort())
                bb.put(data)
            }else if (string.isEmpty()) {
                bb.putShort(0.toShort())
            } else {
                val data = string.toByteArray()
                bb.putShort(data.size.toShort())
                bb.put(data)
            }
        }

        @JvmStatic
        fun marshall32(bb: ByteBuffer, data: ByteArray?) {
            if (data != null) {
                bb.putInt(data.size)
                bb.put(data)
            } else {
                bb.putInt(0)
            }
        }

        @JvmStatic
        fun unMarshallByteArray(bb: ByteBuffer): ByteArray? {
            return try {
                var data: ByteArray? = null
                val byteLen: Short = bb.getShort()
                if (byteLen < 0) {
                    throw Exception("byteLen < 0")
                }
                if (byteLen > 0) {
                    data = ByteArray(byteLen.toInt())
                    bb.get(data)
                    data
                } else {
                    null
                }
            } catch (e: BufferUnderflowException) {
                throw Exception(e)
            }
        }

        @JvmStatic
        fun unMarshall32ByteArray(bb: ByteBuffer): ByteArray? {
            return try {
                var data: ByteArray? = null
                val byteLen: Int = bb.getInt()
                if (byteLen < 0) {
                    throw Exception("byteLen < 0")
                }
                if (byteLen > 0) {
                    data = ByteArray(byteLen)
                    bb.get(data)
                    data
                } else {
                    null
                }
            } catch (e: BufferUnderflowException) {
                throw Exception(e)
            }
        }


        @JvmStatic
        fun calcMarshallSize(byteArray: ByteArray?): Int {
            return if (byteArray != null) {
                2 + byteArray.size
            } else 2
        }

        @JvmStatic
        fun calcMarshallSize(string: String?): Int {
            return if (string != null) {
                Short.SIZE_BYTES + string.toByteArray().size
            } else Short.SIZE_BYTES
        }

        @JvmStatic
        fun <T> marshall(bb: ByteBuffer, data: Collection<T>, elemClass: Class<T>): ByteBuffer? {
            var bb: ByteBuffer = bb
            if (data == null || data.size == 0) {
                bb.putInt(0)
            } else {
                bb.putInt(data.size)
                for (elem in data) {
                    if (elemClass == Int::class.java) {
                        bb.putInt(elem as Int)
                    } else if (elemClass == Short::class.java) {
                        bb.putShort(elem as Short)
                    } else if (elemClass == Byte::class.java) {
                        bb.put(elem as Byte)
                    } else if (elemClass == Long::class.java) {
                        bb.putLong(elem as Long)
                    } else if (elemClass == String::class.java) {
                        marshall(bb, elem as String)
                    } else if (elemClass == ByteArray::class.java) {
                        marshall(bb, elem as ByteArray)
                    } else if (elem is IMarshallable) {
                        bb = (elem as IMarshallable).marshall(bb)
                    } else {
                        throw RuntimeException("unable to marshal element of class " + elemClass.name)
                    }
                }
            }
            return bb
        }

        @JvmStatic
        fun <T> unMarshall(bb: ByteBuffer, data: MutableCollection<T?>, elemClass: Class<T>) {
            try {
                val size: Int = bb.getInt()
                for (i in 0 until size) {
                    var elem: T? = null
                    if (elemClass == Int::class.java) {
                        elem = Integer.valueOf(bb.getInt()) as T
                    } else if (elemClass == Short::class.java) {
                        elem = java.lang.Short.valueOf(bb.getShort()) as T
                    } else if (elemClass == Byte::class.java) {
                        elem = java.lang.Byte.valueOf(bb.get()) as T
                    } else if (elemClass == Long::class.java) {
                        elem = java.lang.Long.valueOf(bb.getLong()) as T
                    } else if (elemClass == String::class.java) {
                        elem = unMarshallShortString(bb) as T?
                    } else if (elemClass == ByteArray::class.java) {
                        elem = unMarshallByteArray(bb) as T?
                    } else {
                        elem = elemClass.newInstance()
                        if (elem is IUnMarshallable) {
                            (elem as IUnMarshallable).unmarshall(bb)
                        } else {
                            Log.e(
                                TAG,
                                "Packer::unMarshall invalid elemClass type " + elemClass.name
                            )
                        }
                    }
                    data.add(elem)
                }
            } catch (e: BufferUnderflowException) {
                throw Exception(e)
            } catch (e: InstantiationException) {
                throw Exception(e)
            } catch (e: IllegalAccessException) {
                throw Exception(e)
            }
        }

        @JvmStatic
        fun <T> calcMarshallSize(data: Collection<T>?): Int {
            var pkgSize = 4
            if (data != null) {
                for (elem in data) {
                    if (elem is Int) {
                        pkgSize += 4
                    } else if (elem is Short) {
                        pkgSize += 2
                    } else if (elem is Byte) {
                        pkgSize += 1
                    } else if (elem is Long) {
                        pkgSize += 8
                    } else if (elem is IMarshallable) {
                        pkgSize += (elem as IMarshallable).size()
                    } else if (elem is String) {
                        pkgSize += calcMarshallSize(elem as String)
                    } else if (elem is ByteArray) {
                        pkgSize += calcMarshallSize(elem as ByteArray)
                    } else {
                        throw IllegalStateException("Packer::calcMarshallSize invalid T type:$elem")
                    }
                }
            }
            return pkgSize
        }

        @JvmStatic
        fun <K, T> marshall(bb: ByteBuffer, data: Map<K, T>?, elemClass: Class<T>): ByteBuffer? {
            var bb: ByteBuffer = bb
            if (data == null || data.size == 0) {
                bb.putInt(0)
            } else {
                bb.putInt(data.size)
                for ((key, elem) in data) {
                    // key
                    if (key is Short) {
                        bb.putShort(key as Short)
                    } else if (key is Int) {
                        bb.putInt(key as Int)
                    } else if (key is Byte) {
                        bb.put(key as Byte)
                    } else if (key is Long) {
                        bb.putLong(key as Long)
                    } else if (key is String) {
                        marshall(bb, key as String)
                    } else if (key is ByteArray) {
                        marshall(bb, key as ByteArray)
                    } else {
                        throw IllegalStateException("marshall Map but unknown key type: " + key!!::class.java.name)
                    }
                    if (elemClass == Int::class.java) {
                        bb.putInt(elem as Int)
                    } else if (elemClass == Short::class.java) {
                        bb.putShort(elem as Short)
                    } else if (elemClass == Byte::class.java) {
                        bb.put(elem as Byte)
                    } else if (elemClass == Long::class.java) {
                        bb.putLong(elem as Long)
                    } else if (elem is IMarshallable) {
                        bb = (elem as IMarshallable).marshall(bb)
                    } else if (elem is String) {
                        //FIXME
                        marshall(bb, elem as String)
                    } else if (elem is ByteArray) {
                        marshall(bb, elem as ByteArray)
                    } else {
                        throw IllegalStateException("marshall Map but unknown value type: " + elem!!::class.java.name)
                    }
                }
            }
            return bb
        }

        @JvmStatic
        fun <K, T> unMarshall(
            bb: ByteBuffer,
            out: MutableMap<K?, T?>,
            keyClass: Class<K>,
            elemClass: Class<T>
        ) {
            try {
                val size: Int = bb.getInt()
                for (i in 0 until size) {
                    // key
                    var key: K? = null
                    key = if (keyClass == Short::class.java) {
                        java.lang.Short.valueOf(bb.getShort()) as K
                    } else if (keyClass == Int::class.java) {
                        Integer.valueOf(bb.getInt()) as K
                    } else if (keyClass == Byte::class.java) {
                        java.lang.Byte.valueOf(bb.get()) as K
                    } else if (keyClass == Long::class.java) {
                        java.lang.Long.valueOf(bb.getLong()) as K
                    } else if (keyClass == ByteArray::class.java) {
                        unMarshallByteArray(bb) as K?
                    } else if (keyClass == String::class.java) {
                        unMarshallShortString(bb) as K?
                    } else {
                        throw IllegalStateException("unMarshall Map but unknown key type: " + keyClass.name)
                    }

                    // value
                    var elem: T? = null
                    if (elemClass == Int::class.java) {
                        elem = Integer.valueOf(bb.getInt()) as T
                    } else if (elemClass == Short::class.java) {
                        elem = java.lang.Short.valueOf(bb.getShort()) as T
                    } else if (elemClass == Byte::class.java) {
                        elem = java.lang.Byte.valueOf(bb.get()) as T
                    } else if (elemClass == Long::class.java) {
                        elem = java.lang.Long.valueOf(bb.getLong()) as T
                    } else if (elemClass == ByteArray::class.java) {
                        elem = unMarshallByteArray(bb) as T?
                    } else if (elemClass == String::class.java) {
                        elem = unMarshallShortString(bb) as T?
                    } else {
                        elem = elemClass.newInstance()
                        if (elem is IUnMarshallable) {
                            (elem as IUnMarshallable).unmarshall(bb)
                        } else {
                            throw IllegalStateException("unMarshall Map but unknown value type: " + elemClass.name)
                        }
                    }
                    out[key] = elem
                }
            } catch (e: BufferUnderflowException) {
                throw Exception(e)
            } catch (e: InstantiationException) {
                throw Exception(e)
            } catch (e: IllegalAccessException) {
                throw Exception(e)
            }
        }

        @JvmStatic
        fun <K, T> calcMarshallSize(data: Map<K, T>?): Int {
            var pkgSize = 4
            if (data != null) {
                for ((key, value) in data) {
                    // key
                    pkgSize += if (key is Short) {
                        2
                    } else if (key is Int) {
                        4
                    } else if (key is Long) {
                        8
                    } else if (key is ByteArray) {
                        calcMarshallSize(key as ByteArray)
                    } else if (key is String) {
                        calcMarshallSize(key as String)
                    } else if (key is Byte) {
                        1
                    } else {
                        throw IllegalStateException("calcMarshallSize Map but unknown key type: " + key!!.javaClass.name)
                    }
                    // value
                    if (value is Int) {
                        pkgSize += 4
                    } else if (value is Short) {
                        pkgSize += 2
                    } else if (value is Long) {
                        pkgSize += 8
                    } else if (value is IMarshallable) {
                        pkgSize += (value as IMarshallable).size()
                    } else if (value is String) {
                        pkgSize += calcMarshallSize(value as String)
                    } else if (value is ByteArray) {
                        pkgSize += calcMarshallSize(value as ByteArray)
                    } else if (value is Byte) {
                        pkgSize += 1
                    } else {
                        throw IllegalStateException("calcMarshallSize Map but unknown value type: $value")
                    }
                }
            }
            return pkgSize
        }

        @JvmStatic
        fun unMarshallShortString(bb: ByteBuffer): String? {
            return try {
                val byteLen: Short = bb.getShort()
                if (byteLen < 0) {
                    throw Exception("byteLen < 0")
                }
                if (byteLen == 0.toShort()) {
                    ""
                }else if (byteLen > 0) {
                    val data = ByteArray(byteLen.toInt())
                    bb.get(data)
                    var ret = String(data)
                    if (ret.equals("null"))
                        null
                    else ret
                } else {
                    null
                }
            } catch (e: BufferUnderflowException) {
                throw Exception(e)
            }
        }

        val STRING_LEN_MAX_LIMIT = 256 // 协议中String的长度限制

        @JvmStatic
        fun limitStringLength(input: String?, length: Int): String? {
            var input = input
            if (TextUtils.isEmpty(input)) {
                return input
            }
            if (input!!.length > length) {
                input = input.substring(0, length)
            }
            return input
        }

        @JvmStatic
        fun <T> limitStringLengthForMap(values: MutableMap<T, String?>?, length: Int) {
            if (values == null || values.size == 0) {
                return
            }
            val iterator: Iterator<T> = values.keys.iterator()
            while (iterator.hasNext()) {
                val key = iterator.next()
                val oldValue = values[key]
                val newValue = limitStringLength(oldValue, length)
                if (!TextUtils.equals(oldValue, newValue)) {
                    values[key] = newValue
                }
            }
        }

        @JvmStatic
        fun dumpUri(inData: ByteBuffer): Int {
            inData.mark()
            var ret = inData.int
            inData.reset()
            return ret
        }

        @JvmStatic
        fun packageToByteBuffer(msg: IMarshallable?): ByteBuffer {
            var bb: ByteBuffer? = null
            msg?.also {
                val size: Int = msg.size()
                bb = ByteBuffer.allocate(size)
                bb!!.order(ByteOrder.LITTLE_ENDIAN)
                bb = msg.marshall(bb!!)
                if (bb!!.position() !== bb!!.capacity()) {
                    Log.e(
                        TAG," marshal wrong size=" + bb!!.capacity() + ", actual=" + bb!!.position()
                    )
                }
            }
            msg?:also {
                bb = ByteBuffer.allocate(Int.SIZE_BYTES)
                bb!!.putInt(-1)
            }
            bb!!.flip()
            return bb!!
        }
    }




}