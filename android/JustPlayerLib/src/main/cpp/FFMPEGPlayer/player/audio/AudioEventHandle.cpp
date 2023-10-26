//
// Created by Peter Xi on 2022/6/2.
//

#include "AudioEventHandle.h"
#include "../../../util/thread/IEventHandler.h"


AudioEventHandle::AudioEventHandle()
:IEventHandler()
{
    std::ostringstream logOs;
    logOs<<"AudioEventHandle::AudioEventHandle";
    LOGD("%s",logOs.str().c_str());

}

AudioEventHandle::~AudioEventHandle()
{
    std::ostringstream logOs;
    logOs<<"AudioEventHandle::~AudioEventHandle";
    LOGD("%s",logOs.str().c_str());
}






