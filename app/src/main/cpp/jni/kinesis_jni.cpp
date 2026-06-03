/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#include "jni/kinesis_jni.h"

#include <sstream>
#include <string>
#include <thread>

namespace kinesis_example {

const char* KinesisJNI::TAG = "KinesisJNI";
JavaVM* KinesisJNI::javaVM = nullptr;
jobject KinesisJNI::class_loader_s = nullptr;
jmethodID KinesisJNI::find_class_method_s = nullptr;

//**********************************************************************
//   C++ CALLED FUNCTIONS START HERE
//**********************************************************************
// Constructor
KinesisJNI::KinesisJNI()
{
  initialized_ = false;
  jni_class_ = nullptr;
}

void KinesisJNI::staticInit(JavaVM* vm)
{
  javaVM = vm;
  JNIEnv* env = getJniEnv();

  if (env)
  {
    auto dummyClass = env->FindClass("com/tomahawkrobotics/kinesis/plugins/salesdemo/KinesisDemoService");
    jclass classClass = env->GetObjectClass(dummyClass);
    auto classLoaderClass = env->FindClass("java/lang/ClassLoader");
    auto getClassLoaderMethod = env->GetMethodID(classClass, "getClassLoader", "()Ljava/lang/ClassLoader;");
    class_loader_s = env->NewGlobalRef(env->CallObjectMethod(dummyClass, getClassLoaderMethod));
    find_class_method_s = env->GetMethodID(classLoaderClass, "findClass", "(Ljava/lang/String;)Ljava/lang/Class;");
  }
}

JNIEnv* KinesisJNI::getJniEnv()
{
  JNIEnv* env = nullptr;
  if (javaVM != nullptr)
  {
    jint res = javaVM->GetEnv((void**)&env, JNI_VERSION_1_6);
    if (res == JNI_EDETACHED)
    {
      if (javaVM->AttachCurrentThread(&env, nullptr) != 0)
      {
        // kinesis::Log::e(TAG, "Could not attach JNI to current thread");
      }
      else
      {
        std::stringstream id;
        id << std::this_thread::get_id();
        // kinesis::Log::i(TAG, "Attaching Thread id #%s", id.str().c_str());
      }
    }
    else if (res != JNI_OK)
    {
      // kinesis::Log::e(TAG, "Java environment is null");
    }
  }
  else
  {
    // kinesis::Log::e(TAG, "JavaVM is null");
  }
  return env;
}

std::string KinesisJNI::JNICopyJStringToString(JNIEnv* env, jstring java_string)
{
  if (java_string == nullptr)
  {
    return std::string();
  }
  const jsize length = env->GetStringLength(java_string);
  jboolean isCopy;

  const char* java_chars = env->GetStringUTFChars(java_string, &isCopy);

  // copy length characters from jchars into a new std::string
  std::string std_string = std::string(java_chars, length);
  env->ReleaseStringUTFChars(java_string, java_chars);

  return std_string;
}

float* KinesisJNI::JNIConvertJfloatArrayToFloatPtr(JNIEnv* env, jfloatArray java_float_array)
{
  jsize length = env->GetArrayLength(java_float_array);
  jboolean isCopy;

  float* cfloat_array = new float[length];
  jfloat* jfloat_elements = env->GetFloatArrayElements(java_float_array, &isCopy);
  for (int i = 0; i < length; i++)
  {
    cfloat_array[i] = jfloat_elements[i];
  }
  env->ReleaseFloatArrayElements(java_float_array, jfloat_elements, JNI_ABORT);
  return cfloat_array;
}

void KinesisJNI::registerThread()
{
  // This validates the JavaVM, Environment, and Attaches the thread if necessary.
  getJniEnv();
}

void KinesisJNI::unregisterThread()
{
  if (javaVM != nullptr)
  {
    std::stringstream id;
    id << std::this_thread::get_id();
    // kinesis::Log::i(TAG, "Detaching Thread id #%s", id.str().c_str());
    javaVM->DetachCurrentThread();
  }
  else
  {
    // kinesis::Log::e(TAG, "JavaVM not set.");
  }
}

jclass KinesisJNI::findClass(const std::string& class_name)
{
  jclass ret_class = nullptr;

  JNIEnv* env = getJniEnv();

  if (env)
  {
    auto jclass_string = env->NewStringUTF(class_name.c_str());
    ret_class = static_cast<jclass>(env->CallObjectMethod(class_loader_s, find_class_method_s, jclass_string));
    if (env->ExceptionOccurred())
    {
      env->ExceptionDescribe();
      ret_class = nullptr;
    }
    env->DeleteLocalRef(jclass_string);
  }
  return ret_class;
}

}  // namespace kinesis_example

/*
 * processing one time initialization:
 *     Cache the javaVM into our context
 *     Make global reference since we are using them from a native thread
 * Note:
 *     All resources allocated here are never released by application
 *     we rely on system to free all global refs when it goes away;
 *     the pairing function JNI_OnUnload() never gets called at all.
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved)
{
  JNIEnv* env;

  kinesis_example::KinesisJNI::staticInit(vm);
  if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK)
  {
    return JNI_ERR;  // KinesisJNI version not supported.
  }

  return JNI_VERSION_1_6;
}
