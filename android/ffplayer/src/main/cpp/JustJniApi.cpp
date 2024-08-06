#include <jni.h>

#include <android/native_window_jni.h>
#include <string>
#include "LogUtil.h"
#include "JniHelper.h"
#include "JustData.h"
#include "base/cpptest/InvokeMain.h"
#include "data/temp/FFPlayer.h"
#include "JustData.h"
#include "MyFFPlayer.h"

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
    MyFFPlayer* player = nullptr;
    player = new MyFFPlayer();
    result = NativeToJavaPoint(player);
    LOGD("create player %ld,%ld",&player,result);
    return result;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_eathemeat_player_ffmpeg_FFPlayer_destory(JNIEnv *env, jobject thiz,jlong jplayer) {
    LOGD("destory player");
    if(jplayer <0) return;
    auto* player = reinterpret_cast<MyFFPlayer*>(jplayer);
    delete player;
}

extern "C"
JNIEXPORT jint JNICALL
Java_com_eathemeat_player_ffmpeg_FFPlayer_setSurface(JNIEnv *env, jobject thiz,jlong handle, jobject android_surface) {
    std::ostringstream logOs;
    if(handle <0 || android_surface == nullptr) logOs<< "handle <0 || android_surface == nullptr\n";
    auto* player = reinterpret_cast<MyFFPlayer*>(handle);
    ANativeWindow *native_window = ANativeWindow_fromSurface(env, android_surface);
    if (!native_window) {
        logOs<<"ANativeWindow_fromSurface: failed\n");
        // do not return fail here;
    }
    player->setSurface(env,native_window);
    if (native_window) ANativeWindow_release(native_window);
    LOGD("%s",logOs.c_str());
    return 1;   //always ok

}

extern "C"
JNIEXPORT jint JNICALL
Java_com_eathemeat_player_ffmpeg_FFPlayer_invoke(JNIEnv *env, jobject thiz,jlong handle,jbyteArray data) {
    std::ostringstream logOs;
    std::string strData = JniHelper::jbyteArray2str(env, data);
    JustPackage::Unpack up(strData.data(), strData.size());
    int ret =-1;
    auto* player = reinterpret_cast<MyFFPlayer*>(handle);
    if(player){
        uint32_t methodId = up.pop_uint32();
        logOs<<" methodId: "<<methodId;
        switch (methodId) {
            case METHOD_SET_DATA_SOURCE: {
                string source = up.pop_varstr32();
                logOs<<"setDataSource:"<< source.c_str();
                ret = player->setDataSource(&source);
            }
                break;
            case METHOD_PREPARE: {
                ret = player->prepare_async();

            }
                break;
            case METHOD_START:{
//                ret = player->start();
            }
                break;
            case METHOD_PAUSE: {
//                ret = player->pause();
            }
                break;
            case METHOD_RESET: {

            }
                break;
            case METHOD_SEEK: {

            }
                break;
            case METHOD_GET_POSITION: {

            }
                break;
            case METHOD_STOP: {

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
    auto* player = reinterpret_cast<MyFFPlayer*>(handle);
    jobject  callback = env->NewLocalRef(jcallback);
//    player->setCallBack(env,callback);
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
