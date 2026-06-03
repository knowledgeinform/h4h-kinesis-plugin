/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "frame_data.h"
#include "sdk/controlled_axes.h"
#include "sdk/quaternion.h"
#include "sdk/vector.h"

#include <functional>
#include <memory>
#include <optional>

namespace kinesis {

/**
 * @brief A class responsible for discovering and commanding coordinate frames on the network.
 *
 * Coordinate frames can be used to observe and control motion by providing a structured reference for measuring
 * position, velocity, acceleration, displacement, and orientation.
 *
 * Example discovery:
 *
 * @note To generate a user interface node, see @ref NodeFactory
 *
 * @code
 * // Step 1: Generate a user interface node using the NodeFactory
 * auto user_node_id = node_factory.constructNode(...);
 *
 * // Step 2: Use the Frame Client to begin discovering the coordinate frames of other nodes on the network
 * kinesis::FrameClient::Config config;
 *
 * config.on_discovery = [](const kinesis::FrameData& data) {
 *     std::cout << "Discovered frame: " << data.frame_id << std::endl;
 * };
 *
 * config.on_update = [](const kinesis::FrameData& data) {
 *     std::cout << "Received frame update" << std::endl;
 * };
 *
 * auto frame_client = kinesis::FrameClient::create(user_node_id, config);
 * @endcode
 */
class FrameClient
{
public:
  /// Provided at construction time to configure the client
  struct Config
  {
    /// Called when a coordinate frame is discovered
    std::function<void(const FrameData&)> on_discovery;

    /// Called when a coordinate frame is updated
    std::function<void(const FrameData&)> on_update;
  };

  /**
   * @brief Constructor
   *
   * @param node_id - The node that will be used to listen to coordinate frames on the network
   * @param config - The configuration containing optional callbacks
   * @return A shared pointer to the created FrameClient instance
   */
  static std::shared_ptr<FrameClient> create(std::string_view node_id, Config config);

  /**
   * @brief Retrieves the FrameData for the specified frame
   *
   * @param frame_id The coordinate frame to retrieve
   * @return The frame data or std::nullopt if no matching frame was found
   */
  virtual std::optional<FrameData> getFrameData(std::string frame_id) const = 0;

  /// Destructor
  virtual ~FrameClient() = default;

  /**
   * @brief Set the linear velocity of a frame to a ratio of its maximum velocity expressed in the frame
   *
   * @param frame_id The ID of the coordinate frame
   * @param velocity_ratio The desired linear velocity (ratio of maximum) expressed as a vector in the given frame. For
   * example, [1, -1, 0] would represent max velocity in the x direction, max negative velocity in the y direction, and
   * zero velocity in the z direction.
   */
  virtual void setLinearVelocityRatioInSelf(const std::string& frame_id, const Vector& velocity_ratio) = 0;

  /**
   * @brief Set the linear velocity of a frame to a ratio of its maximum velocity expressed in its parent frame
   *
   * @param frame_id The ID of the coordinate frame
   * @param velocity_ratio The desired linear velocity (ratio of maximum) expressed as a vector in the parent of the
   * given frame. For example, [1, -1, 0] would represent max velocity in the x direction, max negative velocity in the
   * y direction, and zero velocity in the z direction.
   */
  virtual void setLinearVelocityRatioInParent(const std::string& frame_id, const Vector& velocity_ratio) = 0;

  /**
   * @brief Set the angular velocity in the self frame
   *
   * @param frame_id The ID of the coordinate frame
   * @param velocity The desired angular velocity (radians/second) expressed as a vector in the given frame
   */
  virtual void setAngularVelocityInSelf(const std::string& frame_id, const Vector& velocity) = 0;

  /**
   * @brief Set the angular velocity in the parent frame
   *
   * @param frame_id The ID of the coordinate frame
   * @param velocity The desired angular velocity (radians/second) expressed as a vector in the parent of the given
   * frame
   */
  virtual void setAngularVelocityInParent(const std::string& frame_id, const Vector& velocity) = 0;

  /**
   * @brief Set the angular displacement in the self frame
   *
   * @param frame_id The ID of the coordinate frame
   * @param displacement The desired angular displacement (radians) expressed as a vector in the given frame
   */
  virtual void setAngularDisplacementInSelf(const std::string& frame_id, const Vector& displacement) = 0;

  /**
   * @brief Set the angular displacement in the parent frame
   *
   * @param frame_id The ID of the coordinate frame
   * @param displacement The desired angular displacement (radians) expressed as a vector in the parent frame of the
   * given frame
   */
  virtual void setAngularDisplacementInParent(const std::string& frame_id, const Vector& displacement) = 0;

  /**
   * @brief Set the angular velocity of a frame to a ratio of its maximum velocity expressed in the frame
   * @note This function can be used with frames that have either steered or free angular axes
   * @param frame_id The ID of the frame
   * @param velocity_ratio The desired angular velocity (ratio of maximum) expressed as a vector in the given frame. For
   * example, [1, -1, 0] would represent max velocity in the x direction, max negative velocity in the y direction, and
   * zero velocity in the z direction.
   */
  virtual void setAngularVelocityRatioInSelf(const std::string& frame_id, const Vector& velocity_ratio) = 0;

  /**
   * @brief Set the angular velocity of a frame to a ratio of its maximum velocity expressed in its parent frame
   * @note This function can only be used with frames that have steered angular axes
   * @param frame_id The ID of the frame
   * @param velocity_ratio The desired angular velocity (ratio of maximum) expressed as a vector in the parent frame of
   * the given frame. For example, [1, -1, 0] would represent max velocity in the x direction, max negative velocity in
   * the y direction, and zero velocity in the z direction.
   */
  virtual void setAngularVelocityRatioInParent(const std::string& frame_id, const Vector& velocity_ratio) = 0;

  /**
   * @brief Set the linear acceleration of a frame to a ratio of its maximum velocity expressed in the frame
   * @note This function can only be used with frames that have steered angular axes
   * @param frame_id The ID of the coordinate frame
   * @param acceleration_ratio The desired linear acceleration (ratio of maximum) expressed as a vector in the given
   * frame. For example, [1, -1, 0] would represent max acceleration in the x direction, max negative acceleration in
   * the y direction, and zero acceleration in the z direction.
   */
  virtual void setLinearAccelerationRatio(const std::string& frame_id, const Vector& acceleration_ratio) = 0;

  /**
   * @brief Set the angular position of a frame
   *
   * @param frame_id The ID of the coordinate frame
   * @param position The desired angular position expressed as the quaternion that rotates from this frame to its parent
   * frame (see @ref Quaternion)
   */
  virtual void setAngularPosition(const std::string& frame_id, const Quaternion& position) = 0;
};

}  // namespace kinesis
