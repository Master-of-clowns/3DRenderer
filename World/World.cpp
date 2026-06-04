#include "World.h"

void World::AddObject(Object&& object) {
    objects_.push_back(std::move(object));
}

void World::AddObject(const Object& object) {
    objects_.push_back(object);
}

bool World::IsEmpty() const {
    return objects_.empty();
}

const std::vector<Object>& World::GetObjects() const {
    return objects_;
}

const std::vector<Light>& World::GetLights() const {
    return lights_;
}

void World::AddLight(const Light& light) {
    lights_.push_back(light);
}

std::vector<Object>::const_iterator World::begin() const {
    return objects_.begin();
}

std::vector<Object>::const_iterator World::end() const {
    return objects_.end();
}