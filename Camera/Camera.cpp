//
// Created by s24b_ on 30.01.2026.
//

#include "Camera.h"
#include <cmath>

#include "SFML/Graphics/Glsl.hpp"

Camera::Camera(){
	position = Vector3d(-0.4, -0.3, 0);
	direction = Vector3d(0, 1, 0);
	up = Vector3d(0, 0, 1);
	fov = M_PI / 2;
	aspect = 16.0 / 9.0;
	near = 0;
	far = 1000;
	ProjectionMatrixNeedsUpdate = true;
	ViewMatrixNeedsUpdate = true;
	UpdateProjectionMatrix();
	UpdateViewMatrix();

}

Camera::Camera(const Vector3d& position, const Vector3d& direction, const Vector3d& up,
				double fovDegrees, double aspectRatio, double nearPlane, double farPlane)
	: position(position), direction(direction.normalized()), up(up.normalized()),
	fov(fovDegrees * M_PI / 180.0), aspect(aspectRatio), near(nearPlane), far(farPlane),
	ProjectionMatrixNeedsUpdate(true), ViewMatrixNeedsUpdate(true){
	UpdateProjectionMatrix();
	UpdateViewMatrix();
}

Matrix4d Camera::GetProjectionMatrix() const{
	// if (ProjectionMatrixNeedsUpdate){
	// 	UpdateProjectionMatrix();
	// }
	return projectionMatrix;
}

Matrix4d Camera::GetViewMatrix() const{
	// if (ViewMatrixNeedsUpdate){
	// 	UpdateViewMatrix();
	// }
	return viewMatrix;
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

Vector3d Camera::WorldToScreen(const Vector3d& worldPoint) const{
	Vector4d camPoint = GetViewMatrix() * Vector4d(worldPoint.x(), worldPoint.y(), worldPoint.z(), 1.0);
	Vector4d clipPoint = GetProjectionMatrix() * camPoint;

	if (clipPoint.w() != 0.0){
		clipPoint /= clipPoint.w();
	}

	return Vector3d(clipPoint.x(), clipPoint.y(), clipPoint.z());
}

std::array<Vector3d, 3> Camera::TransformTriangle(const std::array<Vector3d, 3>& triangleVertices) const{
	std::array<Vector3d, 3> screenVertices;
	for (int i = 0; i < 3; ++i){
		screenVertices[i] = WorldToScreen(triangleVertices[i]);
	}
	return screenVertices;
}
