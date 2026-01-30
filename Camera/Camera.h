//
// Created by s24b_ on 30.01.2026.
//

#ifndef CAMERA_H
#define CAMERA_H

#include <Eigen/Dense>

using namespace Eigen;

class Camera{
public:
	Camera();

	Camera(const Vector3d& position, const Vector3d& direction, const Vector3d& up,
			double fovDegrees, double aspectRatio, double nearPlane, double farPlane);

	Matrix4d GetProjectionMatrix() const;
	Matrix4d GetViewMatrix() const;
	Vector3d WorldToScreen(const Vector3d& worldPoint) const;
	std::array<Vector3d, 3> TransformTriangle(const std::array<Vector3d, 3>& triangleVertices) const;

	void SetPosition(const Vector3d& pos){
		position = pos;
		ViewMatrixNeedsUpdate = true;
	}

	void SetDirection(const Vector3d& dir){
		direction = dir.normalized();
		ViewMatrixNeedsUpdate = true;
	}

	void SetUp(const Vector3d& upVec){
		up = upVec.normalized();
		ViewMatrixNeedsUpdate = true;
	}

	void UpdateProjectionMatrix();
	void UpdateViewMatrix();

private:
	Vector3d position;
	Vector3d direction;
	Vector3d up;

	double fov;
	double aspect;
	double near;
	double far;

	Matrix4d projectionMatrix;
	Matrix4d viewMatrix;
	bool ProjectionMatrixNeedsUpdate;
	bool ViewMatrixNeedsUpdate;


};


#endif //CAMERA_H
