#ifndef _JNIHelper_h_
#define _JNIHelper_h_

#include <jni.h>
#include <string>

namespace JniHelper {

    std::string jbyteArray2str(JNIEnv *env, jbyteArray &jstr);

    jbyteArray str2jbyteArray(JNIEnv *env, const std::string &str);

    jbyteArray buf2jbyteArray(JNIEnv *env, const void *buf, uint32_t bufSize);

    jstring str2jstring(JNIEnv *env, const std::string &str);

    jlong NativeToJavaPoint(void *point);

    void *JavaPoint2Native(jlong);

    jobject newGlobalRef(jobject object);

    void deleteGlobalRef(void *object);

    jclass FindClass(const char *name);

    jclass FindClassAsGlobalRef(const char *class_sign);

    jmethodID GetMethodID(jclass clazz, const char *method_name, const char *method_sign);

    jmethodID GetStaticMethodID(jclass clazz, const char *method_name, const char *method_sign);

    jfieldID GetFieldID(jclass clazz, const char *field_name, const char *field_sign);

    jfieldID GetStaticFieldID(jclass clazz, const char *field_name, const char *field_sign);

    int GetSystemAndroidApiLevel();

    JNIEnv *attachJVM();

    void detachJVM();
} // namespace JniHelper

struct JavaMethodTable {
    jmethodID &method;
    const char *name;
    const char *sign;
};

void setJVM(JavaVM *vm);

JavaVM *getJVM();

#define ATTACH_JVM(jni_env)                                            \
  JNIEnv* g_env = NULL;                                                \
  int env_status = getJVM()->GetEnv((void**)&g_env, JNI_VERSION_1_6);  \
  bool bShouldDetach = false;                                          \
  if (env_status == JNI_EDETACHED)                                     \
  {                                                                    \
    jint attachResult = getJVM()->AttachCurrentThread(&jni_env, NULL); \
    if (attachResult >= 0)                                             \
      bShouldDetach = true;                                            \
    else                                                               \
      jni_env = NULL;                                                  \
  }                                                                    \
  else if (JNI_OK == env_status)                                       \
  {                                                                    \
    jni_env = g_env;                                                   \
  }                                                                    \
  else                                                                 \
  {                                                                    \
    jni_env = NULL;                                                    \
  }

#define DETACH_JVM(jni_env)            \
  {                                    \
    if (bShouldDetach)                 \
    {                                  \
      getJVM()->DetachCurrentThread(); \
    }                                  \
  }

#endif
