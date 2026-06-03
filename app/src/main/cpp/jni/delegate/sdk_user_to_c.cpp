/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */
#include "jni/kinesis_jni.h"
#include "jni/sdk_user_handle.h"
#include "sdk_user/kinesis_demo_sdk_user.h"

#include <jni.h>
#include <string>

//**********************************************************************
//   C++ CALLED FUNCTIONS START HERE
//**********************************************************************

namespace kinesis_example {

std::shared_ptr<KinesisDemoSdkUser> g_SdkUser;

}

//**********************************************************************
//   JAVA CALLED FUNCTIONS BELOW HERE
//**********************************************************************

extern "C" JNIEXPORT jboolean JNICALL
Java_com_tomahawkrobotics_kinesis_plugins_salesdemo_KinesisDemoService_startSdk(JNIEnv* env, jclass clazz)
{
  bool success = false;
  if (!kinesis_example::g_SdkUser)
  {
    try
    {
      kinesis_example::g_SdkUser = std::make_shared<kinesis_example::KinesisDemoSdkUser>();
      success = true;
    }
    catch (...)
    {
      kinesis_example::kDelegateJniLog.v << "Exception in SDK Startup";
    }
  }
  // Failure to allocate will cause a seg-fault.  This is ok since we cannot recover from
  // this memory limitation sort of error anyway.
  return success;
}

extern "C" JNIEXPORT void JNICALL
Java_com_tomahawkrobotics_kinesis_plugins_salesdemo_KinesisDemoService_stopSdk(JNIEnv* env, jclass clazz)
{
  kinesis_example::g_SdkUser.reset();
}

extern "C" JNIEXPORT void JNICALL
Java_com_tomahawkrobotics_kinesis_plugins_salesdemo_KinesisDemoService_startNode(JNIEnv* env,
                                                                                 jclass clazz,
                                                                                 jint type_id,
                                                                                 jstring pName,
                                                                                 jstring pType,
                                                                                 jstring pcontrolPort,
                                                                                 jstring pstatusPort,
                                                                                 jstring pcamURL)
{
  if (kinesis_example::g_SdkUser)
  {
    int node_type = (int)type_id;
    std::string name = kinesis_example::KinesisJNI::JNICopyJStringToString(env, pName);
    std::string type = kinesis_example::KinesisJNI::JNICopyJStringToString(env, pType);
    std::string controlPort = kinesis_example::KinesisJNI::JNICopyJStringToString(env, pcontrolPort);
    std::string statusPort = kinesis_example::KinesisJNI::JNICopyJStringToString(env, pstatusPort);
    std::string camURL = kinesis_example::KinesisJNI::JNICopyJStringToString(env, pcamURL);
    kinesis_example::g_SdkUser->startNode(node_type, name, type, controlPort, statusPort, camURL);
  }
  // If the delegate isn't running, LOG messages won't work.
}
