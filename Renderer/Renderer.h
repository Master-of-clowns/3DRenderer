#pragma once

#include "Camera.h"
#include "../World/World.h"
#include "Screen.h"
#include "../Abstract/Color.h"

#include <Eigen/Dense>

using namespace Eigen;

class Renderer {
public:
    Renderer() = default;

    Screen Render(const World& world, const Camera& camera, Screen& screen);

private:
    Color lineColor_ = Color(1.0, 1.0, 1.0);
    Color backgroundColor_ = Color(0.0, 0.0, 0.0);

    Matrix4d ComputeProjectionMatrix(const Camera& camera) const;
    Matrix4d ComputeViewMatrix(const Camera& camera) const;
    Vector3d WorldToNdc(const Vector3d& worldPoint, const Matrix4d& view, const Matrix4d& proj) const;
    std::array<Vector3d, 3> TransformTriangle(const std::array<Vector3d, 3>& vertices,
                                              const Matrix4d& view,
                                              const Matrix4d& proj) const;

    std::vector<Triangle> ProjectTriangles(const Camera& camera, const World& world);
    void DrawTriangles(Screen& screen, const std::vector<Triangle>& projectedTriangles);

    void DrawLine(Screen& screen, int x0, int y0, int x1, int y1);
    void NdcToPixel(const Vector3d& ndc, int screenWidth, int screenHeight, int& outX, int& outY) const;
};
