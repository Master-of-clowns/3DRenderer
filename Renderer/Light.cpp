#include "Light.h"

Light::Light(const Vector3d& position, const Color& color, double intensity, double range)
    : position(position), color(color), intensity(intensity), range(range) {
}

const Vector3d& Light::GetPosition() const {
    return position;
}

const Color& Light::GetColor() const {
    return color;
}

double Light::GetIntensity() const {
    return intensity;
}

double Light::GetRange() const {
    return range;
}

void Light::SetPosition(const Vector3d& p) {
    position = p;
}

void Light::SetColor(const Color& c) {
    color = c;
}

void Light::SetIntensity(double i) {
    intensity = i;
}

void Light::SetRange(double r) {
    range = r;
}
