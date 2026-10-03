//
// Created by Peter Xi on 2022/6/4.
//

#ifndef ANDROIDTEST_FFCONFIG_H
#define ANDROIDTEST_FFCONFIG_H

#include "FFdef.h"

static void* s_instance;

class FFConfig {
private:
    FFConfig()
    :dump_format(true)
    ,genpts(true)
    ,seek_by_bytes(-1)
    ,loop(1)
    ,autoexit(1)
    ,audio_codec_name("")
    ,video_codec_name("")
    ,subtitle_codec_name(""){

    };
    ~FFConfig() {

    };



public:
    static FFConfig* Instant() {
        if(s_instance)
            s_instance = new FFConfig();
        return static_cast<FFConfig *>(s_instance);
    };

    bool dump_format;
    bool genpts;    //generate pts
    int seek_by_bytes; //seek by bytes 0=off 1=on -1=auto
    int loop = 1;   //"set number of times the playback shall be looped", "loop count"
    int autoexit;   //"exit at the end", ""

    //force
    const char *audio_codec_name;
     const char *subtitle_codec_name;
     const char *video_codec_name;


};

#endif //ANDROIDTEST_FFCONFIG_H
