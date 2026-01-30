#ifndef WORLD_H
#define WORLD_H

#include "../Figures/Triangle.h"
#include <vector>
using namespace std;

class World {
public:
    vector<Triangle> triangles;

    World();

    World(int n);

    void AddTriangle(Triangle triangle);

    void SetTriangles(vector<Triangle> triangles);
};



#endif
