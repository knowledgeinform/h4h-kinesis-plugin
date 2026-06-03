/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk_user/kinesis_demo_sdk_user.h"

#include <memory>

namespace kinesis_example {
/**
 * Shared pointer because it's passed to the KinesisDemoSdkUser for callback
 */
extern std::shared_ptr<KinesisDemoSdkUser> g_SdkUser;

static constexpr kinesis::Logger kDelegateJniLog = kinesis::Logger("SdkUserJni");

}  // namespace kinesis_example
