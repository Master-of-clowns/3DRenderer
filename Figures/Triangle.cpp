#include "Triangle.h"

Triangle::Triangle(Vector3d p1, Vector3d p2, Vector3d p3)
    : vertex1(p1), vertex2(p2), vertex3(p3),

      normal((p2 - p1).cross(p3 - p1).normalized()) {

    normal1 = normal;
    normal2 = normal;
    normal3 = normal;
}

std::array<Vector3d, 3> Triangle::GetVertices() const {
    return {vertex1, vertex2, vertex3};
}

const Vector3d& Triangle::GetNormal() const {
    return normal;
}

std::array<Vector3d, 3> Triangle::GetVertexNormals() const {
    return {normal1, normal2, normal3};
}

void Triangle::SetVertexNormal(int index, const Vector3d& n) {
    if (index == 0) {
        normal1 = n;
    } else if (index == 1) {
        normal2 = n;
    } else if (index == 2) {
        normal3 = n;
    }
}
