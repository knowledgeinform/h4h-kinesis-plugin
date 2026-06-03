/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include <jni.h>
#include <string>

namespace kinesis_example {
// Inherit from this abstract class for each unique java object you must talk to.
class KinesisJNI
{
public:
  /**
   * Static initialization of the JNI interface - this should be called ONCE globally from JNI_OnLoad
   * @param vm the Java virtual machine
   */
  static void staticInit(JavaVM* vm);

  /**
   * Abstract class initialization
   */
  virtual void init() = 0;

  // KinesisJNI Support Functions
  static JNIEnv* getJniEnv();
  static float* JNIConvertJfloatArrayToFloatPtr(JNIEnv* env, jfloatArray java_float_array);
  static std::string JNICopyJStringToString(JNIEnv* env, jstring java_string);
  static void registerThread();
  static void unregisterThread();

  /**
   * Find a custom Java class from this application
   * See https://stackoverflow.com/questions/13263340/findclass-from-any-thread-in-android-jni
   * @param class_name The fully qualified Java class name, e.g. com/example/ExampleClass
   * @return the Java class, or NULL if one was not found
   */
  static jclass findClass(const std::string& class_name);

  // Debug TAG
  static const char* TAG;

protected:
  /**
   * Constructor
   */
  KinesisJNI();

  /**
   * Java Virtual machine global handle
   */
  static JavaVM* javaVM;

  /**
   * Java class loader reference used for finding custom Java classes
   */
  static jobject class_loader_s;
  /**
   * Java class method reference used for finding custom Java classes
   */
  static jmethodID find_class_method_s;

  /**
   * Register Java owner to allow push data
   */
  jclass jni_class_ = nullptr;

  /**
   * Flag indicating that the class has been initialized
   */
  bool initialized_ = false;
};
}  // namespace kinesis_example
