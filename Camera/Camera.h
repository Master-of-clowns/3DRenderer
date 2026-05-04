//
// Created by s24b_ on 30.01.2026.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Dense>

using namespace Eigen;

class Camera {
public:
    Camera() = default;

    Camera(const Vector3d& position, const Vector3d& direction, const Vector3d& up,
           double fovDegrees, double aspectRatio, double nearPlane, double farPlane);

    const Matrix4d& GetProjectionMatrix();
    const Matrix4d& GetViewMatrix();
    Vector3d WorldToScreen(const Vector3d& worldPoint);
    std::array<Vector3d, 3> TransformTriangle(const std::array<Vector3d, 3>& triangleVertices);

    void SetPosition(const Vector3d& pos);
    void SetDirection(const Vector3d& dir);
    void SetUp(const Vector3d& upVec);

private:
    Matrix4d projectionMatrix;
    Matrix4d viewMatrix;

    Vector3d position = Vector3d(-0.8, -20, 10);
    Vector3d direction = Vector3d(0, 1, 0);
    Vector3d up = Vector3d(0, 0, 1);
    double fov = M_PI / 2;
    double aspect = 16.0 / 9.0;
    double near = 0;
    double far = 1000;
    bool ProjectionMatrixNeedsUpdate = true;
    bool ViewMatrixNeedsUpdate = true;

    void UpdateProjectionMatrix();
    void UpdateViewMatrix();
};

#endif //CAMERA_H