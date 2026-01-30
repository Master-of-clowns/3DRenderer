//
// Created by s24b_ on 29.01.2026.
//

#include "Triangle.h"

Triangle::Triangle(Vector3d p1, Vector3d p2, Vector3d p3){
	vertex1 = p1;
    vertex2 = p2;
    vertex3 = p3;
    normal = (p1 - p2).cross(p3 - p1);
};

std::array<Vector3d, 3> Triangle::GetVertices() const {
	return {vertex1, vertex2, vertex3};
}

Vector3d Triangle::GetNormal(){
	return normal;
}