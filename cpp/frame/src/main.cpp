#include <iostream>

#include "ned_frame.hpp"

// Test code
int main()
{
    // Reference NEDFrame: Latitude 37.5665, Longitude 126.9780
    NEDFrame frame(37.5665, 126.9780);

    auto test_pose = WGS84Pose{ 37.5665, 126.9780, 180.0 };
    auto test_ned = frame.ToNEDPose(test_pose);
    auto mat = frame.GetTransform(test_pose);
    std::cout << "Transform Matrix:\n" << mat << std::endl;
    std::cout << "NED Position: " << test_ned.position.x() << ", " << test_ned.position.y() << ", " << test_ned.position.z()
              << std::endl;
    std::cout << "Orientation (Quaternion): " << test_ned.orientation.coeffs().transpose() << std::endl;

    // Body Frame Pose: Latitude 37.5667, Longitude 126.9790, Bearing 45 degrees
    WGS84Pose body_wgs_pose{ 37.5643, 126.9780, 45.0 };

    // WGS84 Pose -> NED conversion
    Pose ned_pose = frame.ToNEDPose(body_wgs_pose);

    // Output conversion results
    std::cout << "NED Position: " << ned_pose.position.transpose() << std::endl;
    std::cout << "Orientation (Quaternion): " << ned_pose.orientation.coeffs().transpose() << std::endl;

    // NED -> WGS84 conversion
    WGS84Pose restored_pose = frame.ToWGS84Pose(ned_pose);

    // Output conversion results
    std::cout << "Restored WGS84 Pose: Latitude = " << restored_pose.latitude << ", Longitude = " << restored_pose.longitude
              << ", Bearing = " << restored_pose.bearing << std::endl;

    return 0;
}
