#include <jni.h>

#include <jni.h>
#include <string>
#include "LogUtil.h"
#include "JniHelper.h"
#include "JustData.h"
#include "base/cpptest/InvokeMain.h"
#include "IJustPlayer.h"
#include "FFMPEGPlayer/player/FFMPEGPlayer.h"

using namespace JniHelper;

jint JNI_OnLoad(JavaVM* jvm,void* reserved) {
    JNIEnv* env;
    if (jvm->GetEnv((void**)&env,JNI_VERSION_1_6) != JNI_OK){
        return -1;
    }
    JniHelper::GetSystemAndroidApiLevel();
    setJVM(jvm);
    LOGD("JVM ON LOADING");
    return JNI_VERSION_1_6;
}

void JNI_OnUnload(JavaVM* jvm,void* reserved) {
    JNIEnv* env;
    if (jvm->GetEnv((void**)&env,JNI_VERSION_1_6) != JNI_OK){
        return ;
    }
    LOGD("JVM ON UNLOADING");
    setJVM(NULL);
}


extern "C"
JNIEXPORT jlong
Java_com_eathemeat_player_JustApi_createPlayer(
        JNIEnv* env,
        jobject /* this */,
        jbyteArray jContext) {
    jlong result = 0L;
    ostringstream log;
    string strData = JniHelper::jbyteArray2str(env, jContext);
    Unpack up(strData.data(),strData.size());
    JustContext data;
    JUST_UNMARSHAL(data,up);
    IJustPlayer* player = nullptr;
    switch (data.playerType) {
        case MediaCodec:
        {

//            player =
            break;
        }
        case FFMPEGPlayer:
        {
//            走ffmpeg
            player = new FFMPEGPlayer();
            LOGD(LOG_TAG,"not implement");
            break;
        }
        default:
            LOGE(LOG_TAG,"Unknown Player Type!!!");
    }


    result = NativeToJavaPoint(player);
    LOGD("create player %ld,%ld",&(*player),result);
    return result;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_eathemeat_player_JustApi_destory(JNIEnv *env, jobject thiz,jlong jplayer) {
    LOGD("destory player");
    if(jplayer <0) return;
    JustPlayer* player = reinterpret_cast<JustPlayer*>(jplayer);
    delete player;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_eathemeat_player_JustApi_setSurface(JNIEnv *env, jobject thiz,jlong handle, jobject it) {
    LOGD("setSurface:method");
    if(handle <0) return;
    JustPlayer* player = reinterpret_cast<JustPlayer*>(handle);
    player->setSurface();
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_eathemeat_player_JustApi_invoke(JNIEnv *env, jobject thiz,jlong handle,jbyteArray data) {
    std::ostringstream logOs;
    std::string strData = JniHelper::jbyteArray2str(env, data);
    JustData::JustPackage::Unpack up(strData.data(), strData.size());
    uint32_t uri = up.pop_uint32();
    logOs<<"uri: "<<uri;
    RET ret = RET_ERR;
    switch (uri) {
        case URI_METHOD:{
            JustPlayer* player = reinterpret_cast<JustPlayer*>(handle);
            if(player){

                ret = player->invoke(up);
            }
            break;
        }
    }
    LOGD("JustApi_invoke:method:%s",logOs.str().c_str());
    return ret;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_eathemeat_player_JustApi_setCallBack(JNIEnv *env, jobject thiz, jlong handle,jobject jcallback) {
    std::ostringstream logOs;
    logOs<<"JustApi_setCallBack";
    JustPlayer* player = reinterpret_cast<JustPlayer*>(handle);
    jobject callback = env->NewGlobalRef(jcallback);
    player->setCallBack(env,callback);
    LOGD("%s",logOs.str().c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_eathemeat_player_JustApi_testmain(JNIEnv *env, jobject thiz, jbyteArray name, jbyteArray desc) {
    std::string nameStr = JniHelper::jbyteArray2str(env,name);
    std::string descStr = JniHelper::jbyteArray2str(env,desc);
    std::ostringstream logOs;
    logOs<<" JustApi_testmain "<<nameStr<<descStr;
    LOGD("%s",logOs.str().c_str());
    InvokeMain *test = new InvokeMain();
    test->createData(nameStr,descStr);
    SafeDelete(test);
}
