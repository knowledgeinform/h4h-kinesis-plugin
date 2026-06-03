/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once
#include "sdk/frame_interface.h"
#include "sdk/logger.h"
#include "sdk_beta/frame_client.h"

#include <chrono>
#define _USE_MATH_DEFINES  // for C++
#include <cmath>
#include <future>

namespace kinesis {

class FrameClientImplTester
{
public:
  /**
   * @param node_id The node used to listen for frame updates on the network
   */
  FrameClientImplTester(std::string_view node_id);

  /**
   * Tests frame client by
   *   1. Creating two test nodes (hovering and fixed wing)
   *   2. Using the {FrameClient} class to discover the test frames
   *   3. Using the {FrameClient} class to set the linear velocity, linear acceleration, angular velocity, angular
   * position, and angular displacement of the test frames
   *   4. Using the {FrameClient} class to detect the updated frames
   */
  void runAllTests();

private:
  /**
   * @brief Blocking method that waits for the promise to be fulfilled within the specified timeout.
   *
   * @param[in/out] promise A std::promise that is expecting to be fulfilled.
   * @param timeout The duration to wait before timing out.
   * @return true if the promise is fulfilled within the allotted time
   */
  bool waitForPromise(std::promise<void>& promise, const std::chrono::steady_clock::duration& timeout);

  /**
   * @brief Constructs a hovering node with the given parameters.
   *
   * @param name The name of the hovering node.
   * @param make The make of the hovering node.
   * @param model The model of the hovering node.
   * @return ID of the constructed hovering node.
   */
  std::string constructHoveringNode(std::string_view name, std::string_view make, std::string_view model);

  /**
   * @brief Constructs a fixed wing node with the given parameters.
   *
   * @param name The name of the fixed wing node.
   * @param make The make of the fixed wing node.
   * @param model The model of the fixed wing node.
   * @return ID of the constructed fixed wing node.
   */
  std::string constructFixedWingNode(std::string_view name, std::string_view make, std::string_view model);

  /**
   * @brief Destroys the node with the given node ID.
   *
   * @param node_id The ID of the node to be destroyed.
   */
  void destroyNode(std::string_view node_id);

  /**
   * @brief Sets the angular velocity for the hovering vehicle platform.
   *
   * @param angular_velocity The angular velocity to set.
   */
  void setHoveringAngularVelocity(kinesis::Vector angular_velocity);

  /**
   * @brief Sets the angular velocity for the fixed wing vehicle platform.
   *
   * @param angular_velocity The angular velocity to set.
   */
  void setFixedWingAngularVelocity(kinesis::Vector angular_velocity);

  /**
   * @brief Sets the angular position of the hovering vehicle platform.
   *
   * @param angular_position The angular position to set.
   */
  void setHoveringAngularPosition(kinesis::Quaternion angular_position);

  /**
   * @brief Sets the linear velocity of the hovering vehicle platform .
   *
   * @param velocity The linear velocity to set.
   */
  void setHoveringLinearVelocity(kinesis::Vector velocity);

  /**
   * @brief Sets the linear velocity for the fixed wing vehicle platform.
   *
   * @param velocity The linear velocity to set.
   */
  void setFixedWingLinearVelocity(kinesis::Vector velocity);

  /** Linear velocity of the platform */
  kinesis::Vector hovering_linear_velocity_;

  /** Linear velocity of the fixed wing */
  kinesis::Vector fixed_wing_linear_velocity_;

  /** Angular velocity of the platform */
  kinesis::Vector hovering_angular_velocity_platform_;

  /** Angular velocity of the gimbal */
  kinesis::Vector hovering_angular_velocity_gimbal_;

  /** Angular velocity of the fixed wing */
  kinesis::Vector fixed_wing_angular_velocity_;

  /** Angular position of the platform */
  kinesis::Quaternion hovering_angular_position_platform_;

  /** Angular position of the gimbal */
  kinesis::Quaternion hovering_angular_position_gimbal_;

  /** Angular position of the fixed wing */
  kinesis::Quaternion fixed_wing_angular_position_;

  /** Frame ID for the platform */
  std::string hovering_platform_frame_id_;

  /** Frame ID for the gimbal */
  std::string hovering_gimbal_frame_id_;

  /** Frame ID for the fixed wing platform */
  std::string fixed_wing_platform_frame_id_;

  /** Linear velocity limits for the hovering platform */
  kinesis::Vector hovering_platform_linear_velocity_limits_ = kinesis::Vector(10.0, 10.0, 4.0);

  /** Angular velocity limits for the hovering platform */
  kinesis::Vector hovering_platform_angular_velocity_limits_ = kinesis::Vector(0.0, 0.0, 2.0);

  /** Angular velocity limits for the hovering gimbal */
  kinesis::Vector hovering_gimbal_angular_velocity_limits_ = kinesis::Vector(0.0, M_PI / 4.0, M_PI / 4.0);

  /** Linear velocity limits for the fixed wing */
  kinesis::Vector fixed_wing_linear_velocity_limits_ = kinesis::Vector(25.0, 0.0, 0.0);

  /** Angular velocity limits for the fixed wing */
  kinesis::Vector fixed_wing_angular_velocity_limits_ = kinesis::Vector(0.0, 1.0, 1.0);

  /** Promises used to verify frame command operation **/
  std::promise<void> discovered_promise;
  std::promise<void> set_linear_velocity_ratio_in_self_promise;
  std::promise<void> set_linear_velocity_ratio_in_parent_promise;
  std::promise<void> set_angular_velocity_in_self_promise;
  std::promise<void> set_angular_velocity_in_parent_promise;
  std::promise<void> set_angular_velocity_ratio_in_self_1_promise;
  std::promise<void> set_angular_velocity_ratio_in_self_2_promise;
  std::promise<void> set_angular_velocity_ratio_in_parent_promise;
  std::promise<void> set_angular_position_promise;
  std::promise<void> set_angular_displacement_in_self_promise;
  std::promise<void> set_angular_displacement_in_parent_promise;
  std::promise<void> set_linear_acceleration_promise;
  std::promise<void> set_steer_direction_promise;

  /** Test Vectors and Quaternions **/
  kinesis::Vector test_linear_velocity_ratio_self_ = { 1.0, 1.0, 0.0 };
  kinesis::Vector test_linear_velocity_ratio_parent_ = { 0.7, 0.7, 0.0 };
  kinesis::Vector test_angular_velocity_self_ = { 0.0, 0.0, -0.5 };
  kinesis::Vector test_angular_velocity_parent_ = { 0.0, 0.0, -0.7 };
  kinesis::Vector test_angular_displacement_self_ = { 0.0, 0.7, 0.0 };
  kinesis::Vector test_angular_displacement_parent_ = { 0.0, 0.3, 0.0 };
  kinesis::Vector test_angular_velocity_ratio_parent_ = { 0.0, 0.0, 0.3333 };
  kinesis::Vector test_angular_velocity_ratio_self_1_ = { 0.0, 0.0, -1.0 };
  kinesis::Vector test_angular_velocity_ratio_self_2_ = { 0.0, 0.0, 0.25 };
  kinesis::Vector test_linear_acceleration_ = { 1.0, 0.0, 0.0 };
  kinesis::Quaternion test_angular_position_ = kinesis::Quaternion::getRotationX(M_PI / 4.0);
  kinesis::Quaternion test_angular_position_center_ = { 1.0, 0.0, 0.0, 0.0 };

  /** Flag indicating that we have already passed the linear velocity in self test */
  bool has_set_lv_ratio_in_self_ = false;

  /** Flag indicating that we have already passed the linear velocity in parent test */
  bool has_set_lv_ratio_in_parent_ = false;

  /** Flag indicating that we have already passed the first angular velocity in self test */
  bool has_set_av_in_self_ = false;

  /** Flag indicating that we have already passed the angular velocity in parent test */
  bool has_set_av_in_parent_ = false;

  /** Flag indicating that we have already passed the second angular velocity in self test */
  bool has_set_av_ratio_in_self_1_ = false;

  /** Flag indicating that we have already passed the angular velocity ratio in self test */
  bool has_set_av_ratio_in_self_2_ = false;

  /** Flag indicating that we have already passed the angular velocity ratio in parent test */
  bool has_set_av_ratio_in_parent_ = false;

  /** Flag indicating that we have already passed the angular position test */
  bool has_set_ap_ = false;

  /** Flag indicating that we have already passed the angular displacement in self test */
  bool has_set_ad_in_self_ = false;

  /** Flag indicating that we have already passed the angular displacement in parent test */
  bool has_set_ad_in_parent_ = false;

  /** Flag indicating that we have already passed the linear acceleration test */
  bool has_set_linear_acceleration_ = false;

  /**
   * @brief Helper to determine if two doubles are approximately equal
   *
   * @param a The first double
   * @param b The second double
   * @param epsilon The maximum allowable difference between the two values to consider them equal. Default is 1e-3.
   * @return true if the values are approximately equal, false otherwise.
   */
  bool approximatelyEqual(double a, double b, double epsilon = 1e-3);

  /**
   * @brief Helper to determine if two quaternions are approximately equal
   *
   * @param a The first quaternion
   * @param b The second quaternion
   * @param epsilon The maximum allowable difference between the two values to consider them equal. Default is 1e-3.
   * @return true if the quaternions are approximately equal, false otherwise.
   */
  bool approximatelyEqual(Quaternion a, Quaternion b, double epsilon = 1e-3);

  static constexpr char kIdTestGimbalFrame[] = "test_gimbal_frame";

  /** Logger **/
  static constexpr Logger kLog = Logger("FrameClientImplTester");

  /** The maximum time allowed for frame data to propagate over the network */
  static constexpr std::chrono::duration kFrameDataReceiveTimeout = std::chrono::seconds(3);

  /** The node that is being used to listen */
  std::string node_id_;

  /** Frame client */
  std::shared_ptr<FrameClient> frame_client_;

  /** Gimbal frame interface */
  kinesis::FrameInterface gimbal_frame_interface_;
};

}  // namespace kinesis
