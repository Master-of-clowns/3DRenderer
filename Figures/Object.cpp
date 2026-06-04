#include "Object.h"
#include <map>

namespace {

struct VertexKey {
    long long x, y, z;

    bool operator<(const VertexKey& o) const {
        if (x != o.x) {
            return x < o.x;
        }
        if (y != o.y) {
            return y < o.y;
        }
        return z < o.z;
    }
};

VertexKey MakeKey(const Vector3d& v) {
    constexpr double kScale = 1e5;
    return VertexKey{
        static_cast<long long>(std::llround(v.x() * kScale)),
        static_cast<long long>(std::llround(v.y() * kScale)),
        static_cast<long long>(std::llround(v.z() * kScale))
    };
}
}

Object::Object(std::vector<Triangle>&& triangles, const Vector3d& position)
    : triangles_(std::move(triangles)), position_(position) {
    ComputeVertexNormals();
}

const std::vector<Triangle>& Object::GetTriangles() const {
    return triangles_;
}

const Vector3d& Object::GetPosition() const {
    return position_;
}

void Object::SetPosition(const Vector3d& position) {
    position_ = position;
}

const Color& Object::GetColor() const {
    return color_;
}

void Object::SetColor(const Color& color) {
    color_ = color;
}

Matrix4d Object::GetModelMatrix() const {
    Matrix4d model = Matrix4d::Identity();
    model(0, 3) = position_.x();
    model(1, 3) = position_.y();
    model(2, 3) = position_.z();
    return model;
}

void Object::ComputeVertexNormals() {

    std::map<VertexKey, Vector3d> accumulated;

    for (const Triangle& tri : triangles_) {
        auto verts = tri.GetVertices();
        const Vector3d& faceNormal = tri.GetNormal();
        for (const Vector3d& v : verts) {
            VertexKey key = MakeKey(v);
            auto it = accumulated.find(key);
            if (it == accumulated.end()) {
                accumulated.emplace(key, faceNormal);
            } else {
                it->second += faceNormal;
            }
        }
    }

    for (auto& [key, normal] : accumulated) {
        if (normal.norm() > 1e-12) {
            normal.normalize();
        }
    }

    for (Triangle& tri : triangles_) {
        auto verts = tri.GetVertices();
        for (int i = 0; i < 3; ++i) {
            VertexKey key = MakeKey(verts[i]);
            tri.SetVertexNormal(i, accumulated[key]);
        }
    }
}
