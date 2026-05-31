#include "Renderer.h"
#include <algorithm>
#include <cmath>

void Renderer::SetRenderMode(RenderMode mode) {
    mode_ = mode;
}

RenderMode Renderer::GetRenderMode() const {
    return mode_;
}

void Renderer::ToggleRenderMode() {
    if (mode_ == RenderMode::Solid) {
        mode_ = RenderMode::Wireframe;
    } else {
        mode_ = RenderMode::Solid;
    }
}

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

Vector4d Renderer::ToClip(const Vector3d& world, const Matrix4d& view, const Matrix4d& proj) const {
    Vector4d p(world.x(), world.y(), world.z(), 1.0);
    return proj * (view * p);
}

Color Renderer::ShadeVertex(const Vector3d& worldPos, const Vector3d& worldNormal,
                            const Color& baseColor, const Light& light) const {
    Vector3d normal = worldNormal;
    if (normal.norm() > 1e-12) {
        normal.normalize();
    }

    Vector3d toLight = light.GetPosition() - worldPos;
    double distance = toLight.norm();
    if (distance > 1e-12) {
        toLight /= distance;
    }

    double lambert = std::max(0.0, normal.dot(toLight));

    double range = light.GetRange();
    double attenuation = 1.0;
    if (range > 1e-9) {
        double ratio = distance / range;
        attenuation = 1.0 / (1.0 + ratio * ratio);
    }

    double diffuseStrength = lambert * light.GetIntensity() * attenuation;

    Color diffuse = baseColor * light.GetColor() * diffuseStrength;
    Color ambientPart = baseColor * ambient_;

    return (ambientPart + diffuse).Clamped();
}

Screen Renderer::Render(const World& world, const Camera& camera, Screen& screen) {
    screen.Clear(backgroundColor_);

    Matrix4d view = ComputeViewMatrix(camera);
    Matrix4d proj = ComputeProjectionMatrix(camera);
    const Light& light = world.GetLight();

    int w = screen.GetWidth();
    int h = screen.GetHeight();

    const bool isWire = (mode_ == RenderMode::Wireframe);

    for (const Object& object : world) {
        Matrix4d model = object.GetModelMatrix();
        const Color& baseColor = object.GetColor();

        Matrix3d normalMatrix = model.block<3, 3>(0, 0);

        for (const Triangle& triangle : object.GetTriangles()) {
            auto localVerts = triangle.GetVertices();
            auto vertexNormals = triangle.GetVertexNormals();

            std::array<ClipVertex, 3> clipVerts;
            bool behindCamera = false;

            for (int i = 0; i < 3; ++i) {
                Vector4d worldH = model * Vector4d(localVerts[i].x(),
                                                   localVerts[i].y(),
                                                   localVerts[i].z(), 1.0);

                if (!isWire) {
                    Vector3d worldPos(worldH.x(), worldH.y(), worldH.z());
                    Vector3d worldNormal = normalMatrix * vertexNormals[i];
                    clipVerts[i].color = ShadeVertex(worldPos, worldNormal, baseColor, light);
                } else {
                    clipVerts[i].color = wireframeColor_;
                }

                clipVerts[i].clip = proj * (view * worldH);

                if (clipVerts[i].clip.w() <= 1e-9) {
                    behindCamera = true;
                    break;
                }
            }

            if (behindCamera) {
                continue;
            }

            std::vector<ClipVertex> polygon = ClipPolygon(
                {clipVerts[0], clipVerts[1], clipVerts[2]});

            if (polygon.size() < 3) {
                continue;
            }

            ShadedVertex first = ToShaded(polygon[0], w, h);
            for (size_t i = 1; i + 1 < polygon.size(); ++i) {
                ShadedVertex b = ToShaded(polygon[i], w, h);
                ShadedVertex c = ToShaded(polygon[i + 1], w, h);
                if (isWire) {
                    DrawTriangleWireframe(screen, first, b, c);
                } else {
                    RasterizeTriangle(screen, first, b, c);
                }
            }
        }
    }

    return screen;
}

Renderer::ShadedVertex Renderer::ToShaded(const ClipVertex& v, int width, int height) const {
    double invW = 1.0 / v.clip.w();
    double ndcX = v.clip.x() * invW;
    double ndcY = v.clip.y() * invW;
    double ndcZ = v.clip.z() * invW;

    ShadedVertex s;
    s.x = (ndcX + 1.0) * 0.5 * width;
    s.y = (1.0 - ndcY) * 0.5 * height;
    s.depth = ndcZ;
    s.color = v.color;
    return s;
}

std::vector<Renderer::ClipVertex> Renderer::ClipPolygon(const std::vector<ClipVertex>& input) const {
    enum Plane { Left, Right, Bottom, Top };

    auto distance = [](const ClipVertex& v, Plane plane) -> double {
        switch (plane) {
            case Left:   return v.clip.x() + v.clip.w();
            case Right:  return v.clip.w() - v.clip.x();
            case Bottom: return v.clip.y() + v.clip.w();
            case Top:    return v.clip.w() - v.clip.y();
        }
        return 0.0;
    };

    auto lerp = [](const ClipVertex& a, const ClipVertex& b, double t) -> ClipVertex {
        ClipVertex r;
        r.clip = a.clip + (b.clip - a.clip) * t;
        r.color = Color(
            a.color.GetR() + (b.color.GetR() - a.color.GetR()) * t,
            a.color.GetG() + (b.color.GetG() - a.color.GetG()) * t,
            a.color.GetB() + (b.color.GetB() - a.color.GetB()) * t
        );
        return r;
    };

    std::vector<ClipVertex> poly = input;

    for (Plane plane : {Left, Right, Bottom, Top}) {
        if (poly.empty()) {
            break;
        }

        std::vector<ClipVertex> output;
        size_t n = poly.size();

        for (size_t i = 0; i < n; ++i) {
            const ClipVertex& current = poly[i];
            const ClipVertex& next = poly[(i + 1) % n];

            double dCurrent = distance(current, plane);
            double dNext = distance(next, plane);

            bool currentInside = dCurrent >= 0.0;
            bool nextInside = dNext >= 0.0;

            if (currentInside) {
                output.push_back(current);
            }

            if (currentInside != nextInside) {
                double t = dCurrent / (dCurrent - dNext);
                output.push_back(lerp(current, next, t));
            }
        }

        poly = std::move(output);
    }

    return poly;
}

void Renderer::RasterizeTriangle(Screen& screen,
                                 const ShadedVertex& a,
                                 const ShadedVertex& b,
                                 const ShadedVertex& c) const {
    int w = screen.GetWidth();
    int h = screen.GetHeight();

    int minX = std::max(0, static_cast<int>(std::floor(std::min({a.x, b.x, c.x}))));
    int maxX = std::min(w - 1, static_cast<int>(std::ceil(std::max({a.x, b.x, c.x}))));
    int minY = std::max(0, static_cast<int>(std::floor(std::min({a.y, b.y, c.y}))));
    int maxY = std::min(h - 1, static_cast<int>(std::ceil(std::max({a.y, b.y, c.y}))));

    if (minX > maxX || minY > maxY) {
        return;
    }

    double area = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
    if (std::abs(area) < 1e-9) {
        return;
    }
    double invArea = 1.0 / area;

    for (int y = minY; y <= maxY; ++y) {
        for (int x = minX; x <= maxX; ++x) {

            double px = x + 0.5;
            double py = y + 0.5;

            double w0 = ((b.x - px) * (c.y - py) - (c.x - px) * (b.y - py)) * invArea;
            double w1 = ((c.x - px) * (a.y - py) - (a.x - px) * (c.y - py)) * invArea;
            double w2 = 1.0 - w0 - w1;

            const double eps = -1e-9;
            if (w0 < eps || w1 < eps || w2 < eps) {
                continue;
            }

            double depth = w0 * a.depth + w1 * b.depth + w2 * c.depth;

            Color color(
                w0 * a.color.GetR() + w1 * b.color.GetR() + w2 * c.color.GetR(),
                w0 * a.color.GetG() + w1 * b.color.GetG() + w2 * c.color.GetG(),
                w0 * a.color.GetB() + w1 * b.color.GetB() + w2 * c.color.GetB()
                );

            screen.DrawPixelDepth(x, y, depth, color);
        }
    }
}

void Renderer::DrawTriangleWireframe(Screen& screen,
                                     const ShadedVertex& a,
                                     const ShadedVertex& b,
                                     const ShadedVertex& c) const {
    int x0 = static_cast<int>(std::round(a.x));
    int y0 = static_cast<int>(std::round(a.y));
    int x1 = static_cast<int>(std::round(b.x));
    int y1 = static_cast<int>(std::round(b.y));
    int x2 = static_cast<int>(std::round(c.x));
    int y2 = static_cast<int>(std::round(c.y));

    DrawLine(screen, x0, y0, x1, y1, wireframeColor_);
    DrawLine(screen, x1, y1, x2, y2, wireframeColor_);
    DrawLine(screen, x2, y2, x0, y0, wireframeColor_);
}

void Renderer::DrawLine(Screen& screen, int x0, int y0, int x1, int y1, const Color& color) const {
    int w = screen.GetWidth();
    int h = screen.GetHeight();

    int dx = std::abs(x1 - x0);
    int dy = -std::abs(y1 - y0);
    int sx;
    int sy;

    if (x0 < x1) {
        sx = 1;
    } else {
        sx = -1;
    }

    if (y0 < y1) {
        sy = 1;
    } else {
        sy = -1;
    }
    int err = dx + dy;

    int x = x0;
    int y = y0;

    while (true) {
        if (x >= 0 && x < w && y >= 0 && y < h) {
            screen.DrawPixelDepth(x, y, -1.0, color);
        }
        if (x == x1 && y == y1) break;
        int e2 = 2 * err;
        if (e2 >= dy) {
            if (x == x1) break;
            err += dy;
            x += sx;
        }
        if (e2 <= dx) {
            if (y == y1) break;
            err += dx;
            y += sy;
        }
    }
}