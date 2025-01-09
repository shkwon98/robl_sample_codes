#pragma once

#include <Eigen/Dense>
#include <cmath>

struct WGS84Pose
{
    double latitude;  // Latitude (degrees)
    double longitude; // Longitude (degrees)
    double bearing;   // True Bearing (degrees)
};

struct Pose
{
    Eigen::Vector3d position;
    Eigen::Quaterniond orientation;
};

/**
 * @class NEDFrame
 * @brief A class to handle transformations between WGS84 coordinates and NED frame.
 */
class NEDFrame
{
public:
    /**
     * @brief Constructor to set the reference WGS84 latitude and longitude to be used as the origin of the NED frame.
     *
     * The x-axis of the NED frame points north, the y-axis points east, and the z-axis points down.
     * The scale of the NED frame is in meters.
     *
     * @param ref_lat Reference latitude (degrees)
     * @param ref_lon Reference longitude (degrees)
     */
    NEDFrame(double ref_lat, double ref_lon)
        : ref_lat_(ref_lat)
        , ref_lon_(ref_lon)
    {
    }

    /**
     * @brief Converts a WGS84 pose to the pose with respect to the NED frame.
     * @param pose The WGS84 pose to convert.
     * @return The corresponding pose in the NED frame with position in meters and orientation as a quaternion.
     */
    Pose ToNEDPose(const WGS84Pose &pose) const
    {
        Eigen::Vector3d position = GetPosition(ref_lat_, ref_lon_, pose.latitude, pose.longitude);
        Eigen::Quaterniond orientation(Eigen::AngleAxisd(pose.bearing * M_PI / 180.0, Eigen::Vector3d::UnitZ()));

        return { position, orientation };
    }

    /**
     * @brief Converts a NED frame pose to WGS84 coordinates.
     * @param pose The NED frame pose to convert.
     * @return The corresponding WGS84 pose.
     */
    WGS84Pose ToWGS84Pose(const Pose &pose) const
    {
        auto bearing = GetYaw(pose.orientation);
        auto [lat, lon] = GetWGS84Position(ref_lat_, ref_lon_, pose.position.norm(), bearing);

        return { lat, lon, bearing };
    }

    /**
     * @brief Returns the transformation matrix from the attached body frame on given WGS84 pose to the NED frame.
     * @param pose The WGS84 pose.
     * @return The 4x4 transformation matrix.
     */
    Eigen::Matrix4d GetTransform(const WGS84Pose &pose) const
    {
        Pose ned_pose = ToNEDPose(pose);

        Eigen::Matrix4d transform = Eigen::Matrix4d::Identity();
        transform.block<3, 3>(0, 0) = ned_pose.orientation.toRotationMatrix();
        transform.block<3, 1>(0, 3) = ned_pose.position;

        return transform;
    }

    /**
     * @brief Calculates the distance between two WGS84 coordinates using the Haversine formula.
     * @param lat_1 Latitude of the first point (degrees)
     * @param lon_1 Longitude of the first point (degrees)
     * @param lat_2 Latitude of the second point (degrees)
     * @param lon_2 Longitude of the second point (degrees)
     * @return The distance between the two points (meters)
     */
    double GetHaversineDistance(double lat_1, double lon_1, double lat_2, double lon_2) const
    {
        double d_lat = (lat_2 - lat_1) * (M_PI / 180.0);
        double d_lon = (lon_2 - lon_1) * (M_PI / 180.0);

        double square_root = std::sqrt(std::sin(d_lat / 2) * std::sin(d_lat / 2) +
                                       std::cos(lat_1 * (M_PI / 180.0)) * std::cos(lat_2 * (M_PI / 180.0)) *
                                           std::sin(d_lon / 2) * std::sin(d_lon / 2));

        return 2 * EARTH_RADIUS * std::asin(square_root);
    }

    /**
     * @brief Calculates the bearing of a target point with respect to a body frame on a reference point.
     * @param lat_ref Latitude of the reference point (degrees)
     * @param lon_ref Longitude of the reference point (degrees)
     * @param lat Latitude of the target point (degrees)
     * @param lon Longitude of the target point (degrees)
     * @return The bearing in degrees with respect to the reference body frame.
     */
    double GetBearing(double lat_ref, double lon_ref, double lat, double lon) const
    {
        double d_lon = (lon - lon_ref) * (M_PI / 180.0);
        lat_ref *= (M_PI / 180.0);
        lat *= (M_PI / 180.0);

        double x = std::cos(lat_ref) * std::sin(lat) - std::sin(lat_ref) * std::cos(lat) * std::cos(d_lon);
        double y = std::sin(d_lon) * std::cos(lat);

        return std::atan2(y, x) * (180.0 / M_PI);
    }

    /**
     * @brief Calculates the position of a target point with respect to a body frame on a reference point.
     * @param lat_ref Latitude of the reference point (degrees)
     * @param lon_ref Longitude of the reference point (degrees)
     * @param lat Latitude of the target point (degrees)
     * @param lon Longitude of the target point (degrees)
     * @return The position (x, y, 0) in meters with respect to the reference body frame.
     */
    Eigen::Vector3d GetPosition(double lat_ref, double lon_ref, double lat, double lon) const
    {
        double dist = GetHaversineDistance(lat_ref, lon_ref, lat, lon);
        double bearing = GetBearing(lat_ref, lon_ref, lat, lon);

        double x = dist * std::cos(bearing * (M_PI / 180.0));
        double y = dist * std::sin(bearing * (M_PI / 180.0));

        return Eigen::Vector3d(x, y, 0.0);
    }

    /**
     * @brief Calculates the WGS84 coordinates of a point given a reference WGS84 point, distance, and bearing.
     * @param lat_ref Latitude of the reference point (degrees)
     * @param lon_ref Longitude of the reference point (degrees)
     * @param dist Distance from the reference point (meters)
     * @param bearing Bearing from the reference point (degrees)
     * @return The WGS84 coordinates of the target point.
     */
    std::pair<double, double> GetWGS84Position(double lat_ref, double lon_ref, float dist, float bearing) const
    {
        dist /= EARTH_RADIUS;
        lat_ref *= (M_PI / 180);
        lon_ref *= (M_PI / 180);
        auto bearing_rad = std::fmod((bearing * (M_PI / 180)) + M_PI, 2 * M_PI) - M_PI;

        auto lat =
            std::asin(std::sin(lat_ref) * std::cos(dist) + std::cos(lat_ref) * std::sin(dist) * std::cos(bearing_rad));
        auto lon = 0.0;
        if (std::cos(lat) == 0.0)
        {
            lon = lon_ref; // endpoint a pole
        }
        else
        {
            lon = std::fmod(lon_ref + std::asin(std::sin(bearing_rad) * std::sin(dist) / std::cos(lat)) + M_PI, 2 * M_PI) -
                  M_PI;
        }
        lat *= (180 / M_PI);
        lon *= (180 / M_PI);

        return std::make_pair(lat, lon);
    }

    /**
     * @brief Returns the yaw angle (bearing) from a quaternion.
     * @param quaternion The quaternion representing the orientation.
     * @return The yaw angle (bearing) in degrees.
     */
    double GetYaw(const Eigen::Quaterniond &quaternion) const
    {
        Eigen::Vector3d euler = quaternion.toRotationMatrix().eulerAngles(0, 1, 2);
        euler.z() *= 180.0 / M_PI;

        if (euler.z() < 0)
        {
            euler.z() += 360.0;
        }

        return euler.z();
    }

private:
    const double EARTH_RADIUS = 6'371'000.0; // Earth radius (m)
    double ref_lat_;                         // Reference latitude
    double ref_lon_;                         // Reference longitude
};