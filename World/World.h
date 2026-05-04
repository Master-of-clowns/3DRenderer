#pragma once

#include "../Figures/Triangle.h"
#include <vector>

class World
{
public:
    World() = default;
    World(std::vector<Triangle>&& triangles);
    void AddTriangle(const Triangle& triangle);
    void ResetTriangles(std::vector<Triangle>&& triangles);
    void AddTriangles(const std::vector<Triangle>& triangles);
    void AddTriangles(std::vector<Triangle>&& triangles);
    bool IsEmpty() const;
    // Дима написал сделать triangles приватными, но чтобы можно было пройтись по этому массиву сделал итераторы.
    // Возможно, не самый лучший вариант, но я придумал так....
    std::vector<Triangle>::const_iterator begin() const;
    std::vector<Triangle>::const_iterator end() const;

private:
    std::vector<Triangle> triangles_;
};
