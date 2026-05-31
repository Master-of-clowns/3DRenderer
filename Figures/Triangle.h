#pragma once

#include <Eigen/Dense>
#include <array>

using namespace Eigen;

class Triangle {
public:
    Triangle() = default;
    Triangle(Vector3d p1, Vector3d p2, Vector3d p3);

    std::array<Vector3d, 3> GetVertices() const;

    const Vector3d& GetNormal() const;

    std::array<Vector3d, 3> GetVertexNormals() const;
    void SetVertexNormal(int index, const Vector3d& normal);

private:
    Vector3d vertex1;
    Vector3d vertex2;
    Vector3d vertex3;

    Vector3d normal;

    Vector3d normal1;
    Vector3d normal2;
    Vector3d normal3;
};