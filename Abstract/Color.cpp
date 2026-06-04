#include "Color.h"
#include <algorithm>

Color::Color(double r, double g, double b)
    : r_(r), g_(g), b_(b) {
}

double Color::GetR() const {
    return r_;
}

double Color::GetG() const {
    return g_;
}

double Color::GetB() const {
    return b_;
}

void Color::SetR(double r) {
    r_ = r;
}

void Color::SetG(double g) {
    g_ = g;
}

void Color::SetB(double b) {
    b_ = b;
}

Color Color::operator*(double k) const {
    return Color(r_ * k, g_ * k, b_ * k);
}

Color Color::operator*(const Color& other) const {
    return Color(r_ * other.r_, g_ * other.g_, b_ * other.b_);
}

Color Color::operator+(const Color& other) const {
    return Color(r_ + other.r_, g_ + other.g_, b_ + other.b_);
}

Color Color::Clamped() const {
    return Color(
        std::clamp(r_, 0.0, 1.0),
        std::clamp(g_, 0.0, 1.0),
        std::clamp(b_, 0.0, 1.0)
        );
}
