#include "Renderer.h"
#include <algorithm>
#include <cmath>

Matrix4d Renderer::ComputeProjectionMatrix(const Camera& camera) const {
    double fov = camera.GetFov();
    double aspect = camera.GetAspect();
    double near = camera.GetNear();
    double far = camera.GetFar();

    double tanHalfFov = std::tan(fov / 2.0);
    double range = far - near;

    Matrix4d projection;
    projection.setZero();
    projection(0, 0) = 1.0 / (aspect * tanHalfFov);
    projection(1, 1) = 1.0 / tanHalfFov;
    projection(2, 2) = -(far + near) / range;
    projection(2, 3) = -2.0 * far * near / range;
    projection(3, 2) = -1.0;
    return projection;
}

Matrix4d Renderer::ComputeViewMatrix(const Camera& camera) const {
    Vector3d zAxis = -camera.GetDirection();
    Vector3d xAxis = camera.GetUp().cross(zAxis).normalized();
    Vector3d yAxis = zAxis.cross(xAxis);

    Matrix4d view;
    view.setIdentity();
    view.block<1, 3>(0, 0) = xAxis.transpose();
    view.block<1, 3>(1, 0) = yAxis.transpose();
    view.block<1, 3>(2, 0) = zAxis.transpose();

    const Vector3d& pos = camera.GetPosition();
    view(0, 3) = -xAxis.dot(pos);
    view(1, 3) = -yAxis.dot(pos);
    view(2, 3) = -zAxis.dot(pos);
    return view;
}

Vector3d Renderer::WorldToNdc(const Vector3d& worldPoint,
                              const Matrix4d& view,
                              const Matrix4d& proj) const {
    Vector4d camPoint = view * Vector4d(worldPoint.x(), worldPoint.y(), worldPoint.z(), 1.0);
    Vector4d clipPoint = proj * camPoint;

    if (clipPoint.w() != 0.0) {
        clipPoint /= clipPoint.w();
    }
    return Vector3d(clipPoint.x(), clipPoint.y(), clipPoint.z());
}

std::array<Vector3d, 3> Renderer::TransformTriangle(const std::array<Vector3d, 3>& vertices,
                                                    const Matrix4d& view,
                                                    const Matrix4d& proj) const {
    std::array<Vector3d, 3> ndc;
    for (int i = 0; i < 3; ++i) {
        ndc[i] = WorldToNdc(vertices[i], view, proj);
    }
    return ndc;
}

Screen Renderer::Render(const World& world, const Camera& camera, Screen& screen) {
    for (int y = 0; y < screen.GetHeight(); ++y) {
        for (int x = 0; x < screen.GetWidth(); ++x) {
            screen.DrawPixel(x, y, Color(backgroundColor_));
        }
    }

    std::vector<Triangle> projected = ProjectTriangles(camera, world);
    DrawTriangles(screen, projected);
    return screen;
}

std::vector<Triangle> Renderer::ProjectTriangles(const Camera& camera, const World& world) {
    Matrix4d view = ComputeViewMatrix(camera);
    Matrix4d proj = ComputeProjectionMatrix(camera);

    std::vector<Triangle> projected;
    for (const Triangle& triangle : world) {
        auto vertices = triangle.GetVertices();
        std::array<Vector3d, 3> ndc = TransformTriangle(vertices, view, proj);

        bool allOutLeft = ndc[0].x() < -1 && ndc[1].x() < -1 && ndc[2].x() < -1;
        bool allOutRight = ndc[0].x() > 1 && ndc[1].x() > 1 && ndc[2].x() > 1;
        bool allOutBottom = ndc[0].y() < -1 && ndc[1].y() < -1 && ndc[2].y() < -1;
        bool allOutTop = ndc[0].y() > 1 && ndc[1].y() > 1 && ndc[2].y() > 1;
        bool allOutNear = ndc[0].z() < -1 && ndc[1].z() < -1 && ndc[2].z() < -1;
        bool allOutFar = ndc[0].z() > 1 && ndc[1].z() > 1 && ndc[2].z() > 1;
        if (allOutLeft || allOutRight || allOutBottom || allOutTop || allOutNear || allOutFar) {
            continue;
        }

        projected.emplace_back(ndc[0], ndc[1], ndc[2]);
    }
    return projected;
}

void Renderer::NdcToPixel(const Vector3d& ndc, int screenWidth, int screenHeight,
                          int& outX, int& outY) const {
    double sx = (ndc.x() + 1.0) * 0.5 * screenWidth;
    double sy = (1.0 - ndc.y()) * 0.5 * screenHeight;
    outX = static_cast<int>(std::round(sx));
    outY = static_cast<int>(std::round(sy));
}

void Renderer::DrawTriangles(Screen& screen, const std::vector<Triangle>& projectedTriangles) {
    int w = screen.GetWidth();
    int h = screen.GetHeight();

    for (const Triangle& triangle : projectedTriangles) {
        auto v = triangle.GetVertices();
        int x0, y0, x1, y1, x2, y2;
        NdcToPixel(v[0], w, h, x0, y0);
        NdcToPixel(v[1], w, h, x1, y1);
        NdcToPixel(v[2], w, h, x2, y2);

        DrawLine(screen, x0, y0, x1, y1);
        DrawLine(screen, x1, y1, x2, y2);
        DrawLine(screen, x2, y2, x0, y0);
    }
}

void Renderer::DrawLine(Screen& screen, int x0, int y0, int x1, int y1) {
    int w = screen.GetWidth();
    int h = screen.GetHeight();

    int dx = std::abs(x1 - x0);
    int dy = -std::abs(y1 - y0);
    int sx = x0 < x1 ? 1 : -1;
    int sy = y0 < y1 ? 1 : -1;
    int err = dx + dy;

    int x = x0;
    int y = y0;

    while (true) {
        if (x >= 0 && x < w && y >= 0 && y < h) {
            screen.DrawPixel(x, y, Color(lineColor_));
        }
        if (x == x1 && y == y1)
            break;
        int e2 = 2 * err;
        if (e2 >= dy) {
            if (x == x1)
                break;
            err += dy;
            x += sx;
        }
        if (e2 <= dx) {
            if (y == y1)
                break;
            err += dx;
            y += sy;
        }
    }
}