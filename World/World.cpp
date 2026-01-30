//
// Created by s24b_ on 29.01.2026.
//

#include "World.h"

World::World(){};

World::World(int n){
	triangles.resize(n);
}

void World::AddTriangle(Triangle triangle){
	triangles.push_back(triangle);
}


void World::SetTriangles(vector<Triangle> triangles){
	this->triangles = triangles;
}
