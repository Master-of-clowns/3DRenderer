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

    double half_tan = std::tan(fov / 2.0);
    double range = far - near;

    Matrix4d projection;
    projection.setZero();
    projection(0, 0) = 1.0 / (aspect * half_tan);
    projection(1, 1) = 1.0 / half_tan;
    projection(2, 2) = -(far + near) / range;
    projection(2, 3) = -2.0 * far * near / range;
    projection(3, 2) = -1.0;
    return projection;
}

Matrix4d Renderer::ComputeViewMatrix(const Camera& camera) const {
    Vector3d z = -camera.GetDirection();
    Vector3d x = camera.GetUp().cross(z).normalized();
    Vector3d y = z.cross(x);

    Matrix4d view;
    view.setIdentity();
    view.block<1, 3>(0, 0) = x.transpose();
    view.block<1, 3>(1, 0) = y.transpose();
    view.block<1, 3>(2, 0) = z.transpose();

    const Vector3d& pos = camera.GetPosition();
    view(0, 3) = -x.dot(pos);
    view(1, 3) = -y.dot(pos);
    view(2, 3) = -z.dot(pos);
    return view;
}

Vector4d Renderer::ToClip(const Vector3d& world, const Matrix4d& view, const Matrix4d& proj) const {
    Vector4d p(world.x(), world.y(), world.z(), 1.0);
    return proj * (view * p);
}

Color Renderer::ShadeVertex(const Vector3d& world_pos, const Vector3d& world_normal,
                            const Color& base_color, const Light& light) const {
    Vector3d normal = world_normal;
    if (normal.norm() > 1e-12) {
        normal.normalize();
    }

    Vector3d toLight = light.GetPosition() - world_pos;
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

    double light_power = lambert * light.GetIntensity() * attenuation;

    Color diffuse = base_color * light.GetColor() * light_power;
    Color ambient = base_color * ambient_;

    return (ambient + diffuse).Clamped();
}

Screen Renderer::Render(const World& world, const Camera& camera, Screen& screen) {
    screen.Clear(background_color_);

    Matrix4d view = ComputeViewMatrix(camera);
    Matrix4d proj = ComputeProjectionMatrix(camera);
    const Light& light = world.GetLight();

    int w = screen.GetWidth();
    int h = screen.GetHeight();

    const bool is_wire = (mode_ == RenderMode::Wireframe);

    for (const Object& object : world) {
        Matrix4d model = object.GetModelMatrix();
        const Color& base_color = object.GetColor();

        Matrix3d normal_matrix = model.block<3, 3>(0, 0);

        for (const Triangle& triangle : object.GetTriangles()) {
            auto local_verts = triangle.GetVertices();
            auto vertexNormals = triangle.GetVertexNormals();

            std::array<ClipVertex, 3> clip_verts;
            bool behind_camera = false;

            for (int i = 0; i < 3; ++i) {
                Vector4d world_h = model * Vector4d(local_verts[i].x(),
                                                   local_verts[i].y(),
                                                   local_verts[i].z(), 1.0);

                if (!is_wire) {
                    Vector3d world_pos(world_h.x(), world_h.y(), world_h.z());
                    Vector3d world_normal = normal_matrix * vertexNormals[i];
                    clip_verts[i].color = ShadeVertex(world_pos, world_normal, base_color, light);
                } else {
                    clip_verts[i].color = frame_color_;
                }

                clip_verts[i].clip = proj * (view * world_h);

                if (clip_verts[i].clip.w() <= 1e-9) {
                    behind_camera = true;
                    break;
                }
            }

            if (behind_camera) {
                continue;
            }

            std::vector<ClipVertex> polygon = ClipPolygon(
                {clip_verts[0], clip_verts[1], clip_verts[2]});

            if (polygon.size() < 3) {
                continue;
            }

            ShadedVertex first = ToShaded(polygon[0], w, h);
            for (size_t i = 1; i + 1 < polygon.size(); ++i) {
                ShadedVertex b = ToShaded(polygon[i], w, h);
                ShadedVertex c = ToShaded(polygon[i + 1], w, h);
                if (is_wire) {
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
    double inv_w = 1.0 / v.clip.w();
    double ndc_x = v.clip.x() * inv_w;
    double ndc_y = v.clip.y() * inv_w;
    double ndc_z = v.clip.z() * inv_w;

    ShadedVertex s;
    s.x = (ndc_x + 1.0) * 0.5 * width;
    s.y = (1.0 - ndc_y) * 0.5 * height;
    s.depth = ndc_z;
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

            double cur_d = distance(current, plane);
            double next_d = distance(next, plane);

            bool cur_inside = cur_d >= 0.0;
            bool next_inside = next_d >= 0.0;

            if (cur_inside) {
                output.push_back(current);
            }

            if (cur_inside != next_inside) {
                double t = cur_d / (cur_d - next_d);
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

    int min_x = std::max(0, static_cast<int>(std::floor(std::min({a.x, b.x, c.x}))));
    int max_x = std::min(w - 1, static_cast<int>(std::ceil(std::max({a.x, b.x, c.x}))));
    int min_y = std::max(0, static_cast<int>(std::floor(std::min({a.y, b.y, c.y}))));
    int max_y = std::min(h - 1, static_cast<int>(std::ceil(std::max({a.y, b.y, c.y}))));

    if (min_x > max_x || min_y > max_y) {
        return;
    }

    double area = (b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y);
    if (std::abs(area) < 1e-9) {
        return;
    }
    double inv_area = 1.0 / area;

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {

            double px = x + 0.5;
            double py = y + 0.5;

            double w0 = ((b.x - px) * (c.y - py) - (c.x - px) * (b.y - py)) * inv_area;
            double w1 = ((c.x - px) * (a.y - py) - (a.x - px) * (c.y - py)) * inv_area;
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

    DrawLine(screen, x0, y0, x1, y1, frame_color_);
    DrawLine(screen, x1, y1, x2, y2, frame_color_);
    DrawLine(screen, x2, y2, x0, y0, frame_color_);
}

// Алгоритм рисования прямых из интернета
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