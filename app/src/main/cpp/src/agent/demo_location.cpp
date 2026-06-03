/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#include "agent/demo_location.h"

#include <math.h>

namespace kinesis_example {

DemoLocation::DemoLocation(const std::string& name,
                           const std::string& description,
                           kinesis::Location::Type type,
                           const kinesis::Location& initial_location,
                           bool support_set,
                           bool auto_set)
  : reported_location_(initial_location)
  , set_enabled_(support_set)
  , auto_set_(auto_set)
{
  location_interface_.name = name;
  location_interface_.description = description;
  location_interface_.type = type;
  location_interface_.get_location = std::bind(&DemoLocation::getLocation, this);
  if (support_set)
  {
    location_interface_.set_location = std::bind(&DemoLocation::setLocation, this, std::placeholders::_1);
    location_interface_.is_set_location_enabled = [this]() { return true == set_enabled_; };
  }
}

DemoLocation::~DemoLocation()
{
}

kinesis::LocationInterface DemoLocation::getLocationInterface() const
{
  return location_interface_;
}

kinesis::Location DemoLocation::getLocation()
{
  std::scoped_lock<std::mutex> lock(reported_location_mutex_);
  return reported_location_;
}

bool DemoLocation::setLocation(const kinesis::Location& new_location)
{
  bool accepted = false;
  if (set_enabled_)
  {
    kLog.v << "setLocation";
    if (auto_set_)
    {
      updateLocation(new_location);
    }
    else
    {
      std::scoped_lock<std::mutex> lock(requested_location_mutex_);
      requested_location_ = new_location;
    }
    accepted = true;
  }
  return accepted;
}

void DemoLocation::updateLocation(const kinesis::Location& new_location)
{
  std::scoped_lock<std::mutex> lock(reported_location_mutex_);
  reported_location_ = new_location;
}

void DemoLocation::enableSetLocation(bool enabled)
{
  if (location_interface_.set_location)
  {
    set_enabled_ = enabled;
  }
}

std::optional<kinesis::Location> DemoLocation::popRequestedLocation()
{
  std::scoped_lock<std::mutex> lock(requested_location_mutex_);
  auto output = requested_location_;
  requested_location_ = std::nullopt;
  return output;
}

}  // namespace kinesis_example
