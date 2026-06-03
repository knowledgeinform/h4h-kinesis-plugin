/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/log_interface.h"
#include "sdk/logger.h"
#include "sdk/thread_registration_interface.h"

#include <memory>
#include <string>

namespace kinesis {

/**
 * Base Management object for using the Kinesis Library.  This object must be created before any other Kinesis
 * Library objects will function properly.  Additionally this object must persist until the Library is no longer
 * needed.  Destruction of this object results in loss of Kinesis Network connection and all created Kinesis
 * entities in the Library.
 */
class LibraryManagement
{
public:
  /**
   * Constructor
   * @param uid - any string input that uniquely identifies this application/hardware combination
   *              This ID is provided to Kinesis as the Identifier for mission module access.
   * @param file_storage_path - location on the local storage device for kinesis to use
   * @param log_interface - interface for handling output log messages from kinesis. Optional parameter. If a log
   * interface is not provided it will write logs to the file_storage_path directory.
   * @param thread_interface - interface for registering threads created by kinesis. Optional parameter.
   */
  LibraryManagement(const std::string& uid,
                    std::string file_storage_path,
                    std::shared_ptr<LogInterface> log_interface = nullptr,
                    std::shared_ptr<ThreadRegistrationInterface> thread_interface = nullptr);

  /**
   * Set the IP address of this kinesis node.  This informs kinesis of it's target interface
   * and is used primarily for TCP Identification and connection between nodes in an on-demand
   * setup.
   * @param ipv4 string form of the ipv4 address
   */
  void setIpV4(const std::string& ipv4) const;

  /// Destructor
  virtual ~LibraryManagement();

  /**
   * Initializes the geospatial service with a provided DTED file path.
   * @param dted_file_path The file name of the DEM VRT file.
   * @return true if the geospatial service is initialized, false otherwise.
   */
  bool initializeGeospatialService(const std::string& dted_file_path);

private:
  static constexpr Logger kLog = Logger("LibraryManagement");
};
}  // namespace kinesis
