/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/logger.h"
#include "sdk/view_interface.h"

#include <chrono>

namespace kinesis_example {

/** @class DemoView
 * This is an interface implementation class for the video stream or view
 */
class DemoView
{
public:
  /**
   * Constructor
   * @param name - visible name of the View
   * @param url - web URL for the video stream
   * @param type - sensor type for this view
   * @param fov_no_zoom - field-of-view with no zoom
   * @param parent_id - ID of parent
   * @param linear_offset - linear offset from parent frame
   * @param angular_offset - angular offset from parent frame
   * @param max_zoom - maximum zoom factor (greater than 1.0 if able to zoom)
   * @param cropable - flag indicating if this view can be safely cropped
   * @param capture_image - flag indicating support for capturing single images
   * @param image_capture_interval - interval to capturing periodic images (0.0 if not supported)
   * @param capture_video - flag indicating support for capturing video
   */
  DemoView(const std::string& name,
           const std::string& url,
           kinesis::ViewInterface::SensorType type,
           kinesis::ViewInterface::FieldOfView fov_no_zoom,
           const std::string& parent_id,
           kinesis::Vector linear_offset = kinesis::Vector(),
           kinesis::Quaternion angular_offset = kinesis::Quaternion(),
           double max_zoom = 1.0,
           bool cropable = false,
           bool capture_image = false,
           double image_capture_interval = 0.0,
           bool capture_video = false);

  /**
   * Destructor
   */
  virtual ~DemoView();

  /**
   * Get the view interface
   * @return ViewInterface for Node Factory
   */
  kinesis::ViewInterface getViewInterface();

  /**
   * Get how many seconds have passed since the start time
   * @param start_time time_point for when to start counting from
   * @return number of seconds passed since start
   */
  long double secondsSince(std::chrono::steady_clock::time_point start_time) const;

  /**
   * Get the field of this view
   * @return FieldOfView object
   */
  kinesis::ViewInterface::FieldOfView getFieldOfView() const;

  /**
   * Handle changes to the subscribers of this view
   * @param subscriber
   */
  void subscriptionNotification(const kinesis::ViewInterface::ViewSubscriber& subscriber) const;

  /**
   * Get whether zoom is enabled
   * @return true if ready to accept zoom commands
   */
  bool isZoomEnabled() const;

  /**
   * Set the zoom ratio
   * @param zoom_ratio - zoom ratio (0 = minimum zoom, 1 = maximum zoom)
   */
  void setZoomRatio(double zoom_ratio);

  /**
   * Set the light ratio
   * @param light_ratio - light ratio (0 = minimum intensity (off), 1 = maximum intensity)
   */
  void setLightRatio(double light_ratio);

private:
  static constexpr kinesis::Logger kLog = kinesis::Logger("DemoView");

  /**
   * Capture a single image of this view to internal storage
   */
  void captureImage() const;

  /**
   * Get whether this view is currently able to capture a single image
   * @return true if captureImage() is enabled
   */
  bool isCaptureImageEnabled() const;

  /**
   * Start or stop capturing images periodically from this view to internal storage
   * @param start - control whether to start (true) or stop (false)
   */
  void captureImages(bool start);

  /**
   * Get information about the status of periodic image capture
   */
  kinesis::ViewInterface::ImageCaptureStatus getImagesCaptureStatus();

  /**
   * Start or stop recording video from this view to internal storage
   * @param start - control whether to start (true) or stop (false)
   */
  void captureVideo(bool start);

  /**
   * Get information about the status of video capture
   */
  kinesis::ViewInterface::VideoCaptureStatus getVideoCaptureStatus();

  /**
   * View interface
   */
  kinesis::ViewInterface view_interface_;

  /**
   * Field-of-view with no zoom
   */
  kinesis::ViewInterface::FieldOfView fov_no_zoom_;

  /**
   * Current zoom ratio
   */
  double zoom_ratio_ = 0.0;

  /**
   * Max zoom factor
   */
  double max_zoom_ = 1.0;

  /**
   * Current light ratio
   */
  double light_ratio_ = 0.0;

  /**
   * Image capture interval (seconds)
   */
  double image_capture_interval_ = 0.0;

  /**
   * Time when periodic image capture was started (std::nullopt if not started)
   */
  std::optional<std::chrono::steady_clock::time_point> time_started_images_capture_;

  /**
   * Time when video capture was started (std::nullopt if not started)
   */
  std::optional<std::chrono::steady_clock::time_point> time_started_video_capture_;
};

}  // namespace kinesis_example
