/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include <functional>
#include <string>
#include <vector>

namespace kinesis {

/**
 * Interface class to allow 3rd party integration of Kinesis Mission Module's Image Processing.
 * Integrators must implement their own translation between this interface and their platform
 * hardware. This is provided to Kinesis through the Node Factory.
 */
struct ImageProcessingInterface
{
  /**
   * Required field for the unique name of this interface.
   */
  std::string name;

  /**
   * Required field to see how many concurrent processing requests
   * can be handled.  If not supplied Kinesis assumes 1.
   * Return value is integer [1,+)
   */
  int max_concurrent_jobs = 1;

  /**
   * Optional method that is called once before this interface is used.
   */
  std::function<void()> initialize;

  /**
   * Optional interface called by Kinesis to request the provided path is processed for
   * continually incoming data.  This interface supportings a URL for video or
   * a filesystem path for streaming still images.  This interface is paired with the
   * stop streaming interface to indicate when the streaming path is no longer providing data.
   * Parameter string path to streaming data provided by Kinesis.  If it is a URL it is a video
   * stream, while if it is a filepath it is streaming still images.
   * Return value string identifier for the job ID.  Kinesis may use this job ID to request
   * a halt to processing.
   */
  std::function<std::string(std::string)> process_stream_path;

  /**
   * Method to get whether the stream path processing interface is available for use.
   * Required function
   * @return true if ready to respond to commands
   */
  std::function<bool()> stream_path_is_enabled;

  /**
   * Optional interface called by Kinesis to request the provided folder is processed for every
   * still image in the folder.
   * Parameter string provided by Kinesis is the filesystem path to the folder
   * Return value string identifier for the job ID.  Kinesis may use this job ID to request
   * a halt to processing, or query processing state.
   */
  std::function<std::string(std::string)> process_photo_path;

  /**
   * Method to get whether photo path interface is available for use.
   * Required function
   * @return true if ready to respond to commands
   */
  std::function<bool()> photo_path_is_enabled;

  /**
   * Optional interface called by Kinesis to request the provided list of video files are processed.
   * Parameter string provided by Kinesis is the absolute file path for each of the video files.
   * Return value string identifier for the job ID.  Kinesis may use this job ID to request
   * a halt to processing, or query processing state.
   */
  std::function<std::string(std::vector<std::string>)> process_video_files;

  /**
   * Method to get whether video file interface is available for use.
   * Required function
   * @return true if ready to respond to commands
   */
  std::function<bool()> video_files_is_enabled;

  /**
   * Optional interface to allow Kinesis to request a halt to processing.  This is required if
   * the optional interface process_stream_path is provided
   * Parameter string is the job ID provided on response to a call to provided functions in either
   * process_url or process_stills
   */
  std::function<void(std::string)> stop_processing;

  /**
   * Optional interface that allows Kinesis to query the job ID list of all active jobs on the
   * system.
   */
  std::function<std::vector<std::string>()> get_job_ids;

  /**
   * Enumeration of all available job states
   */
  enum class JobState
  {
    kProcessing,
    kComplete,
    kError,
    kUnknown
  };

  /**
   * Optional interface to allow Kinesis to query the state of processing.  If not provided Kinesis
   * must always assume processing is COMPLETE as it has no other method of determining this state.
   * Processing completion is determined by the integrator, not a direct correlation to the
   * stop_processing call provided by Kinesis.
   * Parameter string is the job ID provided on response to a call to provided processing start
   * functions above.
   * Return value JobState enumeration of valid job states
   */
  std::function<JobState(std::string)> get_job_state;
};

}  // namespace kinesis
