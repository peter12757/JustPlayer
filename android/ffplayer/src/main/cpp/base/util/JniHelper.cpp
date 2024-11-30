#include "JniHelper.h"
#include <stdlib.h>

namespace JniHelper {
    std::string jbyteArray2str(JNIEnv *env, jbyteArray &jarray) {
        char *rtn = 0;
        jsize alen = env->GetArrayLength(jarray);
        jbyte *ba = env->GetByteArrayElements(jarray, JNI_FALSE);
        if (alen > 0) {
            rtn = (char *) malloc(alen + 1);
            memcpy(rtn, ba, alen);
            rtn[alen] = 0;
        }
        env->ReleaseByteArrayElements(jarray, ba, 0);
        std::string stemp;
        stemp.assign(rtn, alen);
        free(rtn);
        return stemp;
    }

    jbyteArray str2jbyteArray(JNIEnv *env, const std::string &str) {
        jbyte *by = (jbyte *) str.c_str();
        jbyteArray jarray = env->NewByteArray(str.length()); // no release
        env->SetByteArrayRegion(jarray, 0, str.length(), by);

        return jarray;
    }

    jbyteArray buf2jbyteArray(JNIEnv *env, const void *buf, uint32_t bufSize) {
        jbyte *by = (jbyte *) buf;
        jbyteArray jarray = env->NewByteArray(bufSize); // no release
        env->SetByteArrayRegion(jarray, 0, bufSize, by);

        return jarray;
    }

    jstring str2jstring(JNIEnv *env, const std::string &str) {
        jsize len = str.size();

        jclass clsstring = env->FindClass("java/lang/String");
        jstring strencode = env->NewStringUTF("GB2312");

        jmethodID mid = env->GetMethodID(clsstring, "<init>", "([BLjava/lang/String;)V");
        jbyteArray barr = env->NewByteArray(len);

        env->SetByteArrayRegion(barr, 0, len, (jbyte *) str.c_str());

        jstring string = (jstring) env->NewObject(clsstring, mid, barr, strencode);

        env->DeleteLocalRef(strencode);
        env->DeleteLocalRef(barr);

        return string;
    }

    jobject newGlobalRef(jobject object) {
        if (object == nullptr) {
            return NULL;
        }
        JNIEnv *env = nullptr;
        ATTACH_JVM(env);
        if (env == nullptr) {
            return NULL;
        }
        jobject ret = env->NewGlobalRef(object);
        DETACH_JVM(env);
        return ret;
    }


    void deleteGlobalRef(void *object) {
        if (object == nullptr) {
            return;
        }
        JNIEnv *env = nullptr;
        ATTACH_JVM(env);
        if (env == nullptr) {
            return;
        }
        env->DeleteGlobalRef((jobject) object);
        DETACH_JVM(env);
    }

    jclass FindClass(const char *name) {
        JNIEnv *env = nullptr;
        ATTACH_JVM(env);
        jclass clazz = env->FindClass(name);
        DETACH_JVM(env)
        return clazz;
    }

    jclass FindClassAsGlobalRef(const char *class_sign) {
        JNIEnv *env = nullptr;
        ATTACH_JVM(env);
        jclass clazz_global = NULL;
        jclass clazz = FindClass(class_sign);
        if (!clazz) {
            DETACH_JVM(env)
            return NULL;
        }

        clazz_global = static_cast<jclass>(newGlobalRef(clazz));
        DETACH_JVM(env)
        return clazz_global;
    }

    jmethodID GetMethodID(jclass clazz, const char *method_name, const char *method_sign) {
        JNIEnv *env = nullptr;
        ATTACH_JVM(env)
        jmethodID method_id = env->GetMethodID(clazz, method_name, method_sign);
        DETACH_JVM(env)
        return method_id;
    }

    jmethodID GetStaticMethodID(jclass clazz, const char *method_name, const char *method_sign) {
        JNIEnv *env = nullptr;
        ATTACH_JVM(env)
        jmethodID method_id = env->GetStaticMethodID(clazz, method_name, method_sign);
        DETACH_JVM(env)
        return method_id;
    }

    jfieldID GetFieldID(jclass clazz, const char *field_name, const char *field_sign) {
        JNIEnv *env = nullptr;
        ATTACH_JVM(env)
        jfieldID field_id = env->GetFieldID(clazz, field_name, field_sign);
        DETACH_JVM(env)
        return field_id;
    }

    jfieldID GetStaticFieldID(jclass clazz, const char *field_name, const char *field_sign) {
        JNIEnv *env = nullptr;
        ATTACH_JVM(env)
        jfieldID field_id = env->GetStaticFieldID(clazz, field_name, field_sign);
        DETACH_JVM(env)
        return field_id;
    }


    static int SDK_INT = 0;  //default

    int GetSystemAndroidApiLevel() {
        if (SDK_INT > 0) return SDK_INT;
        JNIEnv *env = nullptr;
        ATTACH_JVM(env)
        const char *name = "SDK_INT";
        const char *sign = "I";
        const char *classNAME = "android/os/Build";
        jclass jBuildOs = FindClass(classNAME);
        jfieldID fieldID = GetStaticFieldID(jBuildOs, name, sign);
        SDK_INT = env->GetStaticIntField(jBuildOs, fieldID);
        DETACH_JVM(env)
        return SDK_INT;
    }

    JNIEnv *attachJVM() {
        JNIEnv *g_env = NULL;
        JNIEnv *jni_env = NULL;
        int env_status = getJVM()->GetEnv((void **) &g_env, JNI_VERSION_1_6);
        if (env_status == JNI_EDETACHED) {
            jint attachResult = getJVM()->AttachCurrentThread(&jni_env, NULL);
            if (attachResult < 0) jni_env = NULL;
        } else if (JNI_OK == env_status) {
            jni_env = g_env;
        } else {
            jni_env = NULL;
        }
        return jni_env;
    }

    void detachJVM() {
        JNIEnv *g_env = NULL;
        int env_status = getJVM()->GetEnv((void **) &g_env, JNI_VERSION_1_6);
        if (JNI_OK == env_status) {
            getJVM()->DetachCurrentThread();
        }
    }

    jlong NativeToJavaPoint(void *ptr) {
//  static_assert(sizeof(ptr)<= sizeof(jlong),"sys err NativeToJavaPoint");
        jlong ret = reinterpret_cast<u_long>(ptr);
        return ret;
    }

} // namespace JniHelper

static JavaVM *g_JavaVM = nullptr;

void setJVM(JavaVM *vm) { g_JavaVM = vm; }

JavaVM *getJVM() { return g_JavaVM; }
