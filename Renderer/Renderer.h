#pragma once

#include "Camera.h"
#include "Light.h"
#include "../World/World.h"
#include "Screen.h"
#include "../Abstract/Color.h"

#include <Eigen/Dense>
#include <array>
#include <vector>

using namespace Eigen;

enum class RenderMode {
    Solid,
    Wireframe
};

class Renderer
{
public:
    Renderer() = default;

    Screen Render(const World& world, const Camera& camera, Screen& screen);

    void SetRenderMode(RenderMode mode);
    RenderMode GetRenderMode() const;
    void ToggleRenderMode();

private:
    Color backgroundColor_ = Color(0.05, 0.05, 0.08);
    // Color ambient_ = Color(0.25, 0.25, 0.28);
    Color ambient_ = Color(0.25, 0.25, 0.28);
    Color wireframeColor_ = Color(1.0, 1.0, 1.0);

    RenderMode mode_ = RenderMode::Solid;

    struct ShadedVertex
    {
        double x = 0.0;
        double y = 0.0;
        double depth = 0.0;
        Color color;
    };

    struct ClipVertex
    {
        Vector4d clip = Vector4d::Zero();
        Color color;
    };

    Matrix4d ComputeProjectionMatrix(const Camera& camera) const;
    Matrix4d ComputeViewMatrix(const Camera& camera) const;

    std::vector<ClipVertex> ClipPolygon(const std::vector<ClipVertex>& input) const;
    ShadedVertex ToShaded(const ClipVertex& v, int width, int height) const;

    Color ShadeVertex(const Vector3d& worldPos, const Vector3d& worldNormal,
                      const Color& baseColor, const Light& light) const;

    void RasterizeTriangle(Screen& screen,
                           const ShadedVertex& a,
                           const ShadedVertex& b,
                           const ShadedVertex& c) const;

    void DrawTriangleWireframe(Screen& screen,
                               const ShadedVertex& a,
                               const ShadedVertex& b,
                               const ShadedVertex& c) const;

    void DrawLine(Screen& screen, int x0, int y0, int x1, int y1, const Color& color) const;

    Vector4d ToClip(const Vector3d& world, const Matrix4d& view, const Matrix4d& proj) const;
};