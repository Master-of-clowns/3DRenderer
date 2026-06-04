#pragma once

#include "Camera.h"
#include <Eigen/Dense>
#include <SFML/Window.hpp>

using namespace Eigen;

class CameraController {
public:
    CameraController() = default;
    CameraController(Camera* camera, const Vector3d& target,
                     double distance, double yaw, double pitch);

    void Update(double deltaSeconds);

    void SetTarget(const Vector3d& target);
    void SetDistance(double distance);
    void SetOrbitSpeed(double radiansPerSecond);
    void SetZoomSpeed(double unitsPerSecond);

private:
    void ApplyToCamera();

    Camera* camera_ = nullptr;

    Vector3d target_ = Vector3d::Zero();
    double distance_ = 200.0;
    double yaw_ = 0.0;
    double pitch_ = 0.3;

    double orbit_speed_ = 1.5;
    double zoom_speed_ = 2;

    double min_distance_ = 1.0;
    double max_distance_ = 5000.0;
    double max_pitch_ = 1.55;
};