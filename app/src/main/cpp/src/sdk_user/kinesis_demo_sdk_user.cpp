/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#include "sdk_user/kinesis_demo_sdk_user.h"

#include "agent/demo_fixed_wing_vehicle.h"
#include "agent/demo_hovering_vehicle.h"
#include "agent/demo_tracked_vehicle.h"
#include "agent/demo_view.h"
#include "sdk/library_management.h"
#include "sdk/platform_interface.h"
#include "sdk/view_interface.h"
#include "agent/h4h_vehicle.h"

#include <algorithm>
#include <iostream>
#include <string>

namespace kinesis_example {

//void KinesisDemoSdkUser::startNode(int type, const kinesis::Location& initial_location)
//{
//  bool success = false;
//  double vehicle_offset =
//      kConstructionSpacing * (hovering_vehicle_counter_ + tracked_vehicle_counter_ + fixed_wing_vehicle_counter_);
//  auto location = initial_location.getRelativeLocation(vehicle_offset, 0);
//  switch (type)
//  {
//    case kNodeTypeDemoHovering:
//    {
//      auto name = DemoHoveringVehicle::kPlatformName + std::to_string(++hovering_vehicle_counter_);
//      success = DemoHoveringVehicle::constructNode(name, location);
//      break;
//    }
//    case kNodeTypeDemoTracked:
//    {
//      auto name = DemoTrackedVehicle::kPlatformName + std::to_string(++tracked_vehicle_counter_);
//      success = DemoTrackedVehicle::constructNode(name, location);
//      break;
//    }
//    case kNodeTypeDemoFixedWing:
//    {
//      auto name = DemoFixedWingVehicle::kPlatformName + std::to_string(++fixed_wing_vehicle_counter_);
//      success = DemoFixedWingVehicle::constructNode(name, location);
//      break;
//    }
//    default:
//      kLog.e << "Unrecognized Demo Agent of type: " << type;
//      break;
//  }
//
//  if (success)
//  {
//    kLog.i << "Started Demo Agent with type: " << type;
//  }
//  else
//  {
//    kLog.e << "Failed to start Demo Agent of type: " << type;
//  }
//}

void KinesisDemoSdkUser::startNode(int type, std::string pName, std::string pType, std::string pControlPort, std::string pStatusPort, std::string pCamURL, const kinesis::Location& initial_location) {
  bool success = false;
  double vehicle_offset =
          kConstructionSpacing * (hovering_vehicle_counter_ + tracked_vehicle_counter_ + fixed_wing_vehicle_counter_ + h4h_vehicle_counter_);
  auto location = initial_location.getRelativeLocation(vehicle_offset, 0);

  switch (type) {
    case kNodeTypeDemoHovering:
    {
      auto name = pName + std::to_string(++hovering_vehicle_counter_);
      success = DemoHoveringVehicle::constructNode(name, location);
//      success = DemoH4HVehicle::constructNode(name);
      break;
    }
    case kNodeTypeDemoTracked:
    {
      auto name = pName + std::to_string(++tracked_vehicle_counter_);
      success = DemoTrackedVehicle::constructNode(name, location);
//      success = DemoH4HVehicle::constructNode(name);
      break;
    }
    case kNodeTypeDemoFixedWing:
    {
      auto name = pName + std::to_string(++fixed_wing_vehicle_counter_);
      success = DemoFixedWingVehicle::constructNode(name, location);
//      success = DemoH4HVehicle::constructNode(name);
      break;
    }
    case kNodeTypeH4H:
    {
//      auto name = "h4h_" + pName + std::to_string(++h4h_vehicle_counter_);
      auto name = "h4h_" + pName;
//      success = DemoFixedWingVehicle::constructNode(name, location);
      success = DemoH4HVehicle::constructNode(name, pType, pControlPort, pStatusPort,pCamURL);
      break;
    }
    default:
      kLog.e << "Unrecognized Demo Agent of type: " << type;
          break;

  }

    if (success)
    {
        kLog.i << "Started Demo Agent with type: " << type;
    }
    else
    {
        kLog.e << "Failed to start Demo Agent of type: " << type;
    }

}

}  // namespace kinesis_example
