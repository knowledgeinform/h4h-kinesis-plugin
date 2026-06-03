/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/frame_interface.h"
#include "sdk/ratio_interface.h"

#include <array>
#include <functional>
#include <memory>
#include <optional>
#include <string>

namespace kinesis {

/**
 * @brief The struct represents a view. Views are an interface for visual data streams such as video feeds. Each view is
 * associated with a specific sensor type and can provide detailed metadata such as field-of-view, resolution, and other
 * properties that describe how the visual data should be handled and presented.
 *
 * Usage example:
 *
 * @code
 * kinesis::ViewInterface view_down;
 * view_down.name = "Down View";
 * view_down.get_stream_url = [this]() { return getDownLookingStream(); };
 * view_down.type = kinesis::ViewInterface::SensorType::kVisible;
 * view_down.get_field_of_view = [this]() { return getFieldOfView(); };
 * view_down.is_croppable = cropable;
 * view_down.handle_subscriber_change = [this](auto h) { subscriptionNotification(h); };
 * view_down.parent_id = kinesis::FrameInterface::kIdPlatform;
 * view_down.parent_offset_linear = kinesis::Vector();
 * view_down.parent_offset_angular = kinesis::Quaternion();
 *
 * // Populate zoom interface (RatioInterface)
 * view_down.zoom.name = name + "_zoom";
 * view_down.zoom.get_is_enabled = []() { return true; };
 * view_down.zoom.get_ratio = [this]() { return zoom_ratio_; };
 * view_down.zoom.set_ratio = [this](auto r) { setZoomRatio(r); };
 *
 * // Populate light interface (RatioInterface)
 * view_down.light.name = name + "_light";
 * view_down.light.get_is_enabled = []() { return true; };
 * view_down.light.get_ratio = [this]() { return light_ratio_; };
 * view_down.light.set_ratio = [this](auto r) { setLightRatio(r); };
 *
 * // Populate image/video capture options
 * if (capture_image)
 * {
 *   view_down.capture_image = [this]() { captureImage(); };
 *   view_down.is_capture_image_enabled = [this]() { return isCaptureImageEnabled(); };
 * }
 *
 * if (image_capture_interval > 0.0)
 * {
 *   view_down.capture_images = [this](bool enable) { captureImages(enable); };
 *   view_down.is_capture_images_enabled = []() { return true; };
 *   view_down.get_image_capture_status = [this]() { return getImagesCaptureStatus(); };
 * }
 *
 * if (capture_video)
 * {
 *   view_down.capture_video = [this](bool enable) { captureVideo(enable); };
 *   view_down.is_capture_video_enabled = []() { return true; };
 *   view_down.get_video_capture_status = [this]() { return getVideoCaptureStatus(); };
 * }
 * @endcode
 */
struct ViewInterface
{
  /**
   * Container for field-of-view
   */
  struct FieldOfView
  {
    /**
     * Constructor
     * @param width - width in degrees
     * @param height - height in degrees
     */
    FieldOfView(double width, double height)
      : width_deg(width)
      , height_deg(height) {};

    /**
     * Width in degrees
     */
    double width_deg = 0.0;

    /**
     * Height in degrees
     */
    double height_deg = 0.0;
  };

  /**
   * Container for video resolution
   */
  struct Resolution
  {
    /**
     * Constructor
     * @param width - width in pixels
     * @param height - height in pixels
     */
    Resolution(double width, double height)
      : width_pixels(width)
      , height_pixels(height) {};

    /**
     * Width in pixels
     */
    int width_pixels = 0;

    /**
     * Height in pixels
     */
    int height_pixels = 0;
  };

  /**
   * Container for subscriber data
   */
  struct ViewSubscriber
  {
    /**
     * Name of the subscriber
     */
    std::string subscriber_name;

    /**
     * IPv4 address of the subscriber
     */
    std::string subscriber_ipv4;

    /**
     * Name of the view being subscribed to
     */
    std::string view_name;

    /**
     * Flag indicating if this is a start subscription (true) or stop subscription (false) notification
     */
    bool is_subscribing;
  };

  /**
   * Enumeration of sensor types
   */
  enum class SensorType
  {
    kUnknown,
    kVisible,
    kInfrared,
    kMultispectral,
    kTimeOfFlight,
    kLidar,
    kXray,
    kOther
  };

  /**
   * Container for capture status of periodic images
   */
  struct ImageCaptureStatus
  {
    /**
     * True if currently capturing images of this view
     */
    bool is_capturing = false;

    /**
     * The number of images captured so far
     */
    std::optional<int> image_count;

    /**
     * The interval (in seconds) between images captures
     */
    std::optional<double> interval;
  };

  /**
   * Container for download status of captured image
   */
  struct ImageDownloadStatus
  {
    /**
     * True if currently downloading image of this view
     */
    bool is_downloading = false;

    /**
     * The progress (in percent) of image download completion
     */
    std::optional<double> progress;
  };

  /**
   * Container for capture status of the video stream
   */
  struct VideoCaptureStatus
  {
    /**
     * True if currently capturing video of this view
     */
    bool is_capturing = false;

    /**
     * The length of time that video has been captured for
     */
    double elapsed_seconds = 0.0;
  };

  /**
   * Required field for the unique name of this view
   */
  std::string name;

  /**
   * Optional field for the framerate (frames / second) of this view
   */
  int framerate = 30;

  /**
   * Required field for the sensor type used to generate this view
   */
  SensorType type = SensorType::kUnknown;

  /**
   * Optional field indicating whether this view can be cropped
   */
  bool is_croppable = false;

  /**
   * Required method to get the stream URL for this view
   * @return URL string
   */
  std::function<std::string()> get_stream_url;

  /**
   * Required method to get the field of this view.
   * Remember to update this if the view can zoom.
   * @return FieldOfView object
   */
  std::function<FieldOfView()> get_field_of_view;

  /**
   * Optional method to get the resolution of this view.
   * @return Resolution object
   */
  std::function<Resolution()> get_resolution;

  /**
   * Optional method to save a single image of this view to the platform's internal storage
   */
  std::function<void()> capture_image;

  /**
   * Method to get whether this view is currently able to capture a single image.
   * Required if capture_image is provided.
   * @return true if capture_image is enabled
   */
  std::function<bool()> is_capture_image_enabled;

  /**
   * Optional method to start or stop capturing images periodically
   * from this view to the platform's internal storage
   * @param start - control whether to start (true) or stop (false)
   */
  std::function<void(bool)> capture_images;

  /**
   * Method to get whether this view is currently able to capture images periodically.
   * Required if capture_images is provided.
   * @return true if capture_images is enabled
   */
  std::function<bool()> is_capture_images_enabled;

  /**
   * Method to get the periodic image capture status for this view.
   * Required if capture_images is provided.
   * @return image capture status
   */
  std::function<ImageCaptureStatus()> get_image_capture_status;

  /**
   * Optional method to start or stop downloading captured image
   * from this view to the platform's internal storage
   * @param start - control whether to start (true) or stop (false)
   */
  std::function<void(bool)> download_image;

  /**
   * Method to get whether this view is currently able to download captured images.
   * Required if download_image is provided.
   * @return true if download_image is enabled
   */
  std::function<bool()> is_download_image_enabled;

  /**
   * Method to get the image download status for this view.
   * Required if download_image is provided.
   * @return image download status
   */
  std::function<ImageDownloadStatus()> get_image_download_status;

  /**
   * Optional method to Start / stop recording video (of this view) to the platform's internal storage
   * @param start - control whether to start (true) or stop (false)
   */
  std::function<void(bool)> capture_video;

  /**
   * Method to get whether this view is currently able to capture video.
   * Required if capture_video is provided.
   * @return true if capture_video is enabled
   */
  std::function<bool()> is_capture_video_enabled;

  /**
   * Method to get the video capture status for this view.
   * Required if capture_video is provided.
   * @return video capture status
   */
  std::function<VideoCaptureStatus()> get_video_capture_status;

  /**
   * Field for the ID of the parent of this view's coordinate frame.
   * Required if parent_offset_linear or parent_offset_angular are provided.
   * Valid options include:
   * - FrameInterface::kIdPlatform (platform frame)
   * - ID returned by NodeFactory::addFrame()
   */
  std::string parent_id;

  /**
   * Optional field for the linear offset of this view's coordinate frame from its parent
   * expressed as a vector in the parent frame (meters).
   */
  std::optional<Vector> parent_offset_linear;

  /**
   * Optional field for the angular offset of this view's coordinate frame from its parent
   * expressed as a quaternion rotation to the parent frame.
   */
  std::optional<Quaternion> parent_offset_angular;

  /**
   * Optional interface to control the zoom of this view.
   * Use this interface to allow the zoom of this view to be controlled.
   */
  RatioInterface zoom;

  /**
   * Optional interface to control a light for this view.
   * Use this interface to allow the illumination of this view to be controlled.
   */
  RatioInterface light;

  /**
   * Callback for the slant range of the view.
   * This is the distance between the view and the cFOV location.
   */
  std::function<std::optional<double>()> get_slant_range;

  /**
   * Optional field for the ID of the LocationInterface for this view's center location (where its
   * line-of-sight intersects the ground).
   */
  std::string center_location_id;

  /**
   * Optional field for the ID of the TouchInterface for this view.
   * Use this handler to capture screen tap interactions for this view.
   */
  std::string touch_interface_id;

  /**
   * Optional method to handle changes to the subscribers of this view.
   * Called whenever a viewer subscribes or unsubscribes.
   */
  std::function<void(const ViewSubscriber&)> handle_subscriber_change;
};

}  // namespace kinesis
