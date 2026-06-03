/**
 * LIMITED LICENSED SOFTWARE -- SEE LICENSE FILE FOR COPYRIGHT and LICENSE DETAILS
 */

#pragma once

#include "sdk/location.h"
#include "sdk/quaternion.h"

#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace kinesis {

/**
 * Provides access to ground elevation using DTED data.
 * The TerrainInterface class provides methods for retrieving elevation data, calculating CFOV (Center Field of View),
 * and generating skittle charts based on Digital Terrain Elevation Data (DTED).
 *
 * Key Features:
 * - Retrieve elevation data for specific coordinates.
 * - Calculate CFOV for a given location and gimbal quaternion.
 * - Generate skittle charts for a specified area and spacing.
 *
 * Usage:
 * - Use the getHomeElevation() method to retrieve elevation data for specific coordinates.
 * - Use the getElevations() method to retrieve elevation data for multiple coordinates.
 * - Use the getCFoV() method to calculate the CFOV for a given location and gimbal quaternion.
 * - Use the getSkittleChart() method to generate skittle charts for a specified area and spacing.
 * - Use the getSoftwareVersion() method to retrieve the software version of the elevation service.
 */
class TerrainInterface
{
public:
  /**
   * Create a Terrain Interface object
   * @return pointer to a new TerrainInterface
   */
  static std::shared_ptr<TerrainInterface> create();

  /**
   * Asynchronously retrieves the elevation for a given latitude and longitude.
   * The elevation result is provided via a callback function once available.
   * If no server is present, the gRPC call will fail, and the callback will be invoked with std::nullopt.
   * @param latitude - The input latitude.
   * @param longitude - The input longitude.
   * @param callback - A function that receives a tuple with `int requestId` and 'std::optional<double>'. If the
   *                   elevation retrieval fails, the callback is invoked with std::nullopt.
   * @return An `int` representing a unique identifier for the request.
   */
  virtual int getElevation(double latitude,
                           double longitude,
                           std::function<void(int request_id, std::optional<double>)> callback) = 0;

  /**
   * Asynchronously retrieves the elevations for multiple latitude and longitude pairs.
   * The elevation results are provided via a callback function once available.
   * If no server is present, the gRPC call will fail, and the callback will be invoked with std::nullopt.
   * @param coordinates - A vector of pairs, where each pair contains a latitude (first) and longitude (second).
   *                      Ensure that the order is consistent with the Kinesis standard for packing location vectors.
   * @param callback - A function that receives a tuple with `int requestId` and 'vector of std::optional<double>'.
   *                   Each element in the vector corresponds to the elevation of the respective
   *                   latitude and longitude pair in the input vector. If the elevation retrieval
   *                   for a specific pair fails, the corresponding element is std::nullopt.
   * @return An `int` representing a unique identifier for the request.
   */
  virtual int getElevations(const std::vector<std::pair<double, double>>& coordinates,
                            std::function<void(int request_id, std::vector<std::optional<double>>)> callback) = 0;

  /**
   * Asynchronously retrieves the CFOV (Camera Field of View) for the gimbal to look at,
   * given a sensor location (latitude, longitude, and altitude) and gimbal quaternion
   * (azimuth, pitch, and roll). The result is provided via a callback.
   * If no server is present, the gRPC call will fail, and the callback will be invoked with std::nullopt.
   * @param sensor_location The Kinesis Location object containing latitude, longitude, and altitude.
   * @param sensor_rotation A quaternion object representing:
   *     - azimuth: The azimuth in degrees west of geographical north.
   *     - pitch: The pitch angle above the horizon.
   *     - roll: The roll angle rolling left from level.
   * @param callback A function that receives a tuple with `int requestId` and
   *                 `std::optional<LocationInterface::Location>`. If the CFOV retrieval is successful, it contains the
   *                 target location. If the retrieval fails, the callback is invoked with `std::nullopt`.
   * @return An `int` representing a unique identifier for the request.
   */
  virtual int getCFoV(Location sensor_location,
                      Quaternion sensor_rotation,
                      std::function<void(int request_id, std::optional<Location>)> callback) = 0;

  /**
   * Asynchronously retrieves the elevation profile for a specified geographic area and spacing.
   * The elevation profile is a grid of points containing elevation data.
   * If no server is present, the gRPC call will fail, and the callback will be invoked with std::nullopt.
   * @param lat_start The start latitude of the area.
   * @param lat_end The end latitude of the area.
   * @param long_start The start longitude of the area.
   * @param long_end The end longitude of the area.
   * @param count The number of slices between two points.
   * @param callback A function that receives a tuple with `int requestId` and `std::optional<std::vector<Location>>`.
   *                 If the elevation profile is successfully generated, the vector contains elevation values.
   *                 If the generation fails, the callback is invoked with `std::nullopt`.
   * @return An `int` representing a unique identifier for the request.
   */
  virtual int getElevationProfile(double lat_start,
                                  double lat_end,
                                  double long_start,
                                  double long_end,
                                   int count,
                                   std::function<void(int request_id, std::optional<std::vector<Location>>)> callback) = 0;

  /**
   * Asynchronously retrieves the software version of the elevation service.
   * The version result is provided via a callback function once available.
   * If no server is present, the gRPC call will fail, and the callback will be invoked with std::nullopt.
   * @param callback - A function that receives the retrieved software version as a
   *                   std::optional<std::string>. If the version retrieval fails,
   *                   the callback is invoked with std::nullopt.
   */
  virtual void getSoftwareVersion(std::function<void(std::optional<std::string>)> callback) = 0;
};
}  // namespace kinesis
