//
// Created by s24b_ on 30.01.2026.
//

#include "Camera.h"
#include <cmath>

#include "../extern/SFML-3.0.2/include/SFML/Graphics/Glsl.hpp"

Camera::Camera(const Vector3d& position, const Vector3d& direction, const Vector3d& up,
               double fovDegrees, double aspectRatio, double nearPlane, double farPlane)
	: position(position), direction(direction.normalized()), up(up.normalized()),
	fov(fovDegrees * M_PI / 180.0), aspect(aspectRatio), near(nearPlane), far(farPlane),
	ProjectionMatrixNeedsUpdate(true), ViewMatrixNeedsUpdate(true){
	UpdateProjectionMatrix();
	UpdateViewMatrix();
}

const Matrix4d& Camera::GetProjectionMatrix() {
	if (ProjectionMatrixNeedsUpdate){
		UpdateProjectionMatrix();
	}
	return projectionMatrix;
}

const Matrix4d& Camera::GetViewMatrix() {
	if (ViewMatrixNeedsUpdate){
		UpdateViewMatrix();
	}
	return viewMatrix;
}

Vector3d Camera::WorldToScreen(const Vector3d& worldPoint) {
	Vector4d camPoint = GetViewMatrix() * Vector4d(worldPoint.x(), worldPoint.y(), worldPoint.z(), 1.0);
	Vector4d clipPoint = GetProjectionMatrix() * camPoint;

	if (clipPoint.w() != 0.0){
		clipPoint /= clipPoint.w();
	}

	return Vector3d(clipPoint.x(), clipPoint.y(), clipPoint.z());
}

std::array<Vector3d, 3> Camera::TransformTriangle(const std::array<Vector3d, 3>& triangleVertices) {
	std::array<Vector3d, 3> screenVertices;
	for (int i = 0; i < 3; ++i){
		screenVertices[i] = WorldToScreen(triangleVertices[i]);
	}
	return screenVertices;
}

void Camera::SetPosition(const Vector3d& pos){
    position = pos;
    ViewMatrixNeedsUpdate = true;
}

void Camera::SetDirection(const Vector3d& dir){
    direction = dir.normalized();
    ViewMatrixNeedsUpdate = true;
}

void Camera::SetUp(const Vector3d& upVec){
    up = upVec.normalized();
    ViewMatrixNeedsUpdate = true;
}

void Camera::UpdateProjectionMatrix(){
    double tanHalfFov = tan(fov / 2.0);
    double range = far - near;

    projectionMatrix.setZero();
    projectionMatrix(0, 0) = 1.0 / (aspect * tanHalfFov);
    projectionMatrix(1, 1) = 1.0 / tanHalfFov;
    projectionMatrix(2, 2) = -(far + near) / range;
    projectionMatrix(2, 3) = -2.0 * far * near / range;
    projectionMatrix(3, 2) = -1.0;

    ProjectionMatrixNeedsUpdate = false;
}

void Camera::UpdateViewMatrix(){
    Vector3d zAxis = -direction;
    Vector3d xAxis = up.cross(zAxis).normalized();
    Vector3d yAxis = zAxis.cross(xAxis);

    viewMatrix.setIdentity();
    viewMatrix.block<1, 3>(0, 0) = xAxis.transpose();
    viewMatrix.block<1, 3>(1, 0) = yAxis.transpose();
    viewMatrix.block<1, 3>(2, 0) = zAxis.transpose();

    viewMatrix(0, 3) = -xAxis.dot(position);
    viewMatrix(1, 3) = -yAxis.dot(position);
    viewMatrix(2, 3) = -zAxis.dot(position);

    ViewMatrixNeedsUpdate = false;
}
