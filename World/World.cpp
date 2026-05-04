//
// Created by s24b_ on 29.01.2026.
//

#include "World.h"

World::World(std::vector<Triangle>&& triangles) {
    triangles_ = std::move(triangles);
}

void World::AddTriangle(const Triangle& triangle) {
    triangles_.push_back(triangle);
}

void World::ResetTriangles(std::vector<Triangle>&& triangles) {
    triangles_ = std::move(triangles);
}

void World::AddTriangles(const std::vector<Triangle>& triangles) {
    triangles_.insert(triangles_.end(), triangles.begin(), triangles.end());
}

void World::AddTriangles(std::vector<Triangle>&& triangles) {
    triangles_.insert(triangles_.end(),
                      std::make_move_iterator(triangles.begin()),
                      std::make_move_iterator(triangles.end()));
}

bool World::IsEmpty() const {
    return triangles_.empty();
}

std::vector<Triangle>::const_iterator World::begin() const {
    return triangles_.begin();
}

std::vector<Triangle>::const_iterator World::end() const {
    return triangles_.end();
}
