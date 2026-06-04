#pragma once

#include "../Figures/Triangle.h"
#include "../Abstract/Color.h"
#include <Eigen/Dense>
#include <vector>

using namespace Eigen;

class Object {
public:
    Object() = default;
    explicit Object(std::vector<Triangle>&& triangles,
                    const Vector3d& position = Vector3d::Zero());

    const std::vector<Triangle>& GetTriangles() const;

    const Vector3d& GetPosition() const;
    void SetPosition(const Vector3d& position);

    const Color& GetColor() const;
    void SetColor(const Color& color);

    Matrix4d GetModelMatrix() const;

private:
    void ComputeVertexNormals();

    std::vector<Triangle> triangles_;
    Vector3d position_ = Vector3d::Zero();
    Color color_ = Color(0.8, 0.8, 0.8);
};
