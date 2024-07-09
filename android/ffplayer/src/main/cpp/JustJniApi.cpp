#include <jni.h>

#include <jni.h>
#include <string>
#include "LogUtil.h"
#include "JniHelper.h"
#include "JustData.h"
#include "base/cpptest/InvokeMain.h"
#include "ffmpeg/player/FFPlayer.h"
#include "JustData.h"

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
Java_com_eathemeat_player_ffmpeg_FFPlayer_createPlayer(
        JNIEnv* env,
        jobject /* this */,
        jbyteArray jContext) {
    jlong result = 0L;
    ostringstream log;
    string strData = JniHelper::jbyteArray2str(env, jContext);
    Unpack up(strData.data(),strData.size());
    JustContext data;
    JUST_UNMARSHAL(data,up);
    FFPlayer* player = nullptr;
            player = new FFPlayer();
    result = NativeToJavaPoint(player);
    LOGD("create player %ld,%ld",&(*player),result);
    return result;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_eathemeat_player_ffmpeg_FFPlayer_destory(JNIEnv *env, jobject thiz,jlong jplayer) {
    LOGD("destory player");
    if(jplayer <0) return;
    auto* player = reinterpret_cast<FFPlayer*>(jplayer);
    delete player;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_eathemeat_player_ffmpeg_FFPlayer_setSurface(JNIEnv *env, jobject thiz,jlong handle, jobject it) {
    LOGD("setSurface:method");
    if(handle <0) return;
    auto* player = reinterpret_cast<FFPlayer*>(handle);
    player->setSurface();
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_eathemeat_player_ffmpeg_FFPlayer_invoke(JNIEnv *env, jobject thiz,jlong handle,jbyteArray data) {
    std::ostringstream logOs;
    std::string strData = JniHelper::jbyteArray2str(env, data);
    JustPackage::Unpack up(strData.data(), strData.size());
    uint32_t uri = up.pop_uint32();
    logOs<<"uri: "<<uri;
    RET ret = RET_ERR;
    switch (uri) {
        case URI_METHOD:{
            auto* player = reinterpret_cast<FFPlayer*>(handle);
            if(player){
                ret = player->invoke(up);
            }
            break;
        }
    }
    LOGD("ffmpeg_FFPlayer_invoke:method:%s",logOs.str().c_str());
    return ret;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_eathemeat_player_ffmpeg_FFPlayer_setCallBack(JNIEnv *env, jobject thiz, jlong handle,jobject jcallback) {
    std::ostringstream logOs;
    logOs<<"ffmpeg_FFPlayer_setCallBack";
    auto* player = reinterpret_cast<FFPlayer*>(handle);
    jobject  callback = env->NewLocalRef(jcallback);
    player->setCallBack(env,callback);
    LOGD("%s",logOs.str().c_str());
}

extern "C"
JNIEXPORT void JNICALL
Java_com_eathemeat_player_ffmpeg_FFPlayer_testmain(JNIEnv *env, jobject thiz, jbyteArray name, jbyteArray desc) {
    std::string nameStr = JniHelper::jbyteArray2str(env,name);
    std::string descStr = JniHelper::jbyteArray2str(env,desc);
    std::ostringstream logOs;
    logOs<<" ffmpeg_FFPlayer_testmain "<<nameStr<<descStr;
    LOGD("%s",logOs.str().c_str());
    InvokeMain *test = new InvokeMain();
    test->createData(nameStr,descStr);
    SafeDelete(test);
}
