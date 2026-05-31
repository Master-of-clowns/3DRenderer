#include "CameraController.h"
#include <algorithm>
#include <cmath>

CameraController::CameraController(Camera* camera, const Vector3d& target,
                                   double distance, double yaw, double pitch)
    : camera_(camera), target_(target), distance_(distance), yaw_(yaw), pitch_(pitch) {
    ApplyToCamera();
}

void CameraController::Update(double deltaSeconds) {
    if (camera_ == nullptr) {
        return;
    }

    double orbitStep = orbitSpeed_ * deltaSeconds;
    double zoomStep = 1 + (zoomSpeed_ - 1) * deltaSeconds;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) {
        yaw_ -= orbitStep;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) {
        yaw_ += orbitStep;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
        pitch_ += orbitStep;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LShift)) {
        pitch_ -= orbitStep;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) {
        distance_ /= zoomStep;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) {
        distance_ *= zoomStep;
    }

    pitch_ = std::clamp(pitch_, -maxPitch_, maxPitch_);
    distance_ = std::clamp(distance_, minDistance_, maxDistance_);

    ApplyToCamera();
}

void CameraController::SetTarget(const Vector3d& target) {
    target_ = target;
    ApplyToCamera();
}

void CameraController::SetDistance(double distance) {
    distance_ = std::clamp(distance, minDistance_, maxDistance_);
    ApplyToCamera();
}

void CameraController::SetOrbitSpeed(double radiansPerSecond) {
    orbitSpeed_ = radiansPerSecond;
}

void CameraController::SetZoomSpeed(double unitsPerSecond) {
    zoomSpeed_ = unitsPerSecond;
}

void CameraController::ApplyToCamera() {
    double cosP = std::cos(pitch_);
    double sinP = std::sin(pitch_);
    double cosY = std::cos(yaw_);
    double sinY = std::sin(yaw_);

    Vector3d offset(
        distance_ * cosP * cosY,
        distance_ * cosP * sinY,
        distance_ * sinP
    );

    Vector3d position = target_ + offset;
    Vector3d direction = (target_ - position).normalized();

    camera_->SetPosition(position);
    camera_->SetDirection(direction);
    camera_->SetUp(Vector3d(0.0, 0.0, 1.0));
}