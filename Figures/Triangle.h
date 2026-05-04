//
// Created by s24b_ on 29.01.2026.
//

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <Eigen/Dense>

using namespace Eigen;

class Triangle {
public:
    Triangle() = default;
    Triangle(Vector3d p1, Vector3d p2, Vector3d p3);
    std::array<Vector3d, 3> GetVertices() const;
    const Vector3d& GetNormal() const;

private:
    Vector3d vertex1;
    Vector3d vertex2;
    Vector3d vertex3;

    Vector3d normal;
};

#endif //TRIANGLE_H