#pragma once

#include <Eigen/Dense>
#include <numbers>

using namespace Eigen;

class Camera
{
public:
    Camera() = default;

    Camera(const Vector3d& position, const Vector3d& direction, const Vector3d& up,
           double fovDegrees, double aspectRatio, double nearPlane, double farPlane);

    const Vector3d& GetPosition() const;
    const Vector3d& GetDirection() const;
    const Vector3d& GetUp() const;
    double GetFov() const;
    double GetAspect() const;
    double GetNear() const;
    double GetFar() const;

    void SetPosition(const Vector3d& pos);
    void SetDirection(const Vector3d& dir);
    void SetUp(const Vector3d& upVec);
    void SetFov(double fovRadians);
    void SetAspect(double aspectRatio);
    void SetNear(double nearPlane);
    void SetFar(double farPlane);

private:
    Vector3d position = Vector3d(0.0, -10.0, 0.0);
    Vector3d direction = Vector3d(0.0, 1.0, 0.0);
    Vector3d up = Vector3d(0.0, 0.0, 1.0);
    double fov = std::numbers::pi / 2.0;
    double aspect = 16.0 / 9.0;
    double near = 0.1;
    double far = 1000.0;
};
