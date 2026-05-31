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

const Light& World::GetLight() const {
    return light_;
}

void World::SetLight(const Light& light) {
    light_ = light;
}

std::vector<Object>::const_iterator World::begin() const {
    return objects_.begin();
}

std::vector<Object>::const_iterator World::end() const {
    return objects_.end();
}