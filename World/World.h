#pragma once

#include "../Figures/Object.h"
#include "../Renderer/Light.h"
#include <vector>

class World
{
public:
    World() = default;

    void AddObject(Object&& object);
    void AddObject(const Object& object);

    bool IsEmpty() const;

    const std::vector<Object>& GetObjects() const;

    const std::vector<Light>& GetLights() const;
    void AddLight(const Light& light);

    std::vector<Object>::const_iterator begin() const;
    std::vector<Object>::const_iterator end() const;

private:
    std::vector<Object> objects_;
    std::vector<Light> lights_;
};
