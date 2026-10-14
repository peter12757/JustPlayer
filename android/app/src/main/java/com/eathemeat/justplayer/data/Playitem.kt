package com.eathemeat.justplayer.data

import java.net.URL

/**
 * author: PeterX
 * time: 2026/10/13 8:34
 */
class Playitem {
    var title:String = ""

    var description:String = ""
    lateinit var url: URL

    constructor(title:String,description:String,url:URL) {
        this.title = title
        this.description = description
        this.url = url
    }
}