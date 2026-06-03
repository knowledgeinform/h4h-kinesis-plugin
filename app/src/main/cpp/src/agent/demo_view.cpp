/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#include "agent/demo_view.h"

#include "sdk/notification.h"

#include <cmath>
#include <iomanip>

namespace kinesis_example {

DemoView::DemoView(const std::string& name,
                   const std::string& url,
                   kinesis::ViewInterface::SensorType type,
                   kinesis::ViewInterface::FieldOfView fov_no_zoom,
                   const std::string& parent_id,
                   kinesis::Vector linear_offset,
                   kinesis::Quaternion angular_offset,
                   double max_zoom,
                   bool cropable,
                   bool capture_image,
                   double image_capture_interval,
                   bool capture_video)
  : fov_no_zoom_(fov_no_zoom)
  , max_zoom_(max_zoom)
  , image_capture_interval_(image_capture_interval)
{
  view_interface_.name = name;
  view_interface_.get_stream_url = [url]() { return url; };
  view_interface_.type = type;
  view_interface_.get_field_of_view = [this]() { return getFieldOfView(); };
  view_interface_.is_croppable = cropable;
  view_interface_.handle_subscriber_change = [this](auto subscriber) { subscriptionNotification(subscriber); };
  view_interface_.parent_id = parent_id;
  view_interface_.parent_offset_linear = linear_offset;
  view_interface_.parent_offset_angular = angular_offset;

  // Populate zoom interface (RatioInterface)
  if (isZoomEnabled())
  {
    view_interface_.zoom.name = name + "_zoom";
    view_interface_.zoom.get_is_enabled = []() { return true; };
    view_interface_.zoom.get_ratio = [this]() { return zoom_ratio_; };
    view_interface_.zoom.set_ratio = [this](auto r) { setZoomRatio(r); };
  }

  // Populate light interface (RatioInterface)
  view_interface_.light.name = name + "_light";
  view_interface_.light.get_is_enabled = []() { return true; };
  view_interface_.light.get_ratio = [this]() { return light_ratio_; };
  view_interface_.light.set_ratio = [this](auto r) { setLightRatio(r); };

  // Populate image/video capture options
  if (capture_image)
  {
    view_interface_.capture_image = [this]() { captureImage(); };
    view_interface_.is_capture_image_enabled = [this]() { return isCaptureImageEnabled(); };
  }
  if (image_capture_interval_ > 0.0)
  {
    view_interface_.capture_images = [this](bool enable) { captureImages(enable); };
    view_interface_.is_capture_images_enabled = []() { return true; };
    view_interface_.get_image_capture_status = [this]() { return getImagesCaptureStatus(); };
  }
  if (capture_video)
  {
    view_interface_.capture_video = [this](bool enable) { captureVideo(enable); };
    view_interface_.is_capture_video_enabled = []() { return true; };
    view_interface_.get_video_capture_status = [this]() { return getVideoCaptureStatus(); };
  }
}

DemoView::~DemoView()
{
}

kinesis::ViewInterface DemoView::getViewInterface()
{
  return view_interface_;
}

kinesis::ViewInterface::FieldOfView DemoView::getFieldOfView() const
{
  kinesis::ViewInterface::FieldOfView fov = fov_no_zoom_;
  if (isZoomEnabled())
  {
    double zoom_factor = 1.0 + (max_zoom_ - 1.0) * zoom_ratio_;
    if (zoom_factor > 1.0)
    {
      fov.width_deg = fov.width_deg / zoom_factor;
      fov.height_deg = fov.height_deg / zoom_factor;
    }
  }
  return fov;
}

bool DemoView::isZoomEnabled() const
{
  return (max_zoom_ > 1.0);
}

void DemoView::setZoomRatio(double zoom_ratio)
{
  if (isZoomEnabled())
  {
    zoom_ratio_ = std::clamp(zoom_ratio, 0.0, 1.0);
    kLog.v << "Zoom ratio set to: " << std::fixed << std::setprecision(2) << zoom_ratio_;
  }
}

void DemoView::setLightRatio(double light_ratio)
{
  light_ratio_ = light_ratio;
  kLog.v << "Light ratio set to: " << std::fixed << std::setprecision(2) << light_ratio_;
}

void DemoView::subscriptionNotification(const kinesis::ViewInterface::ViewSubscriber& subscriber) const
{
  kLog.d << "Subscriber " << subscriber.subscriber_name << " from " << subscriber.subscriber_ipv4 << " is "
         << (subscriber.is_subscribing ? "now" : "stopping") << " listening";
}

void DemoView::captureImage() const
{
  // Pretend to capture an image
  kLog.d << "Image Capture Received";
}

bool DemoView::isCaptureImageEnabled() const
{
  // Don't allow single image capture if periodically capturing images
  return view_interface_.capture_image && (std::nullopt == time_started_images_capture_);
}

void DemoView::captureImages(bool enable)
{
  if (time_started_images_capture_.has_value() != enable)
  {
    if (enable)
    {
      time_started_images_capture_ = std::chrono::steady_clock::now();
    }
    else
    {
      time_started_images_capture_ = std::nullopt;
    }
  }
}

kinesis::ViewInterface::ImageCaptureStatus DemoView::getImagesCaptureStatus()
{
  kinesis::ViewInterface::ImageCaptureStatus status;
  status.is_capturing = time_started_images_capture_.has_value();
  status.interval = image_capture_interval_;
  if (time_started_images_capture_.has_value())
  {
    double seconds = secondsSince(time_started_images_capture_.value());
    if (auto interval = status.interval)
    {
      // avoid divide by zero
      if (interval.value() > 0)
      {
        status.image_count = std::round(seconds / interval.value()) + 1;
      }
    }
  }
  return status;
}

long double DemoView::secondsSince(std::chrono::steady_clock::time_point start_time) const
{
  return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - start_time).count() /
         1000000.0;
}

void DemoView::captureVideo(bool enable)
{
  if (time_started_video_capture_.has_value() != enable)
  {
    if (enable)
    {
      time_started_video_capture_ = std::chrono::steady_clock::now();
    }
    else
    {
      time_started_video_capture_ = std::nullopt;
    }
  }
}

kinesis::ViewInterface::VideoCaptureStatus DemoView::getVideoCaptureStatus()
{
  kinesis::ViewInterface::VideoCaptureStatus status;
  status.is_capturing = time_started_video_capture_.has_value();
  if (time_started_video_capture_.has_value())
  {
    status.elapsed_seconds = secondsSince(time_started_images_capture_.value());
  }
  return status;
}

}  // namespace kinesis_example
