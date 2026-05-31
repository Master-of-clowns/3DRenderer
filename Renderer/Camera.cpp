#include "Camera.h"

Camera::Camera(const Vector3d& position, const Vector3d& direction, const Vector3d& up,
               double fovDegrees, double aspectRatio, double nearPlane, double farPlane)
    : position(position),
      direction(direction.normalized()),
      up(up.normalized()),
      fov(fovDegrees * std::numbers::pi / 180.0),
      aspect(aspectRatio),
      near(nearPlane),
      far(farPlane) {
}

const Vector3d& Camera::GetPosition() const {
    return position;
}

const Vector3d& Camera::GetDirection() const {
    return direction;
}

const Vector3d& Camera::GetUp() const {
    return up;
}

double Camera::GetFov() const {
    return fov;
}

double Camera::GetAspect() const {
    return aspect;
}

double Camera::GetNear() const {
    return near;
}

double Camera::GetFar() const {
    return far;
}

void Camera::SetPosition(const Vector3d& pos) {
    position = pos;
}

void Camera::SetDirection(const Vector3d& dir) {
    direction = dir.normalized();
}

void Camera::SetUp(const Vector3d& upVec) {
    up = upVec.normalized();
}

void Camera::SetFov(double fovRadians) {
    fov = fovRadians;
}

void Camera::SetAspect(double aspectRatio) {
    aspect = aspectRatio;
}

void Camera::SetNear(double nearPlane) {
    near = nearPlane;
}

void Camera::SetFar(double farPlane) {
    far = farPlane;
}
