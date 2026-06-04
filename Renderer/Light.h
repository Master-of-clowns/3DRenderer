#pragma once

#include "../Abstract/Color.h"
#include <Eigen/Dense>

using namespace Eigen;

class Light {
public:
    Light() = default;

    Light(const Vector3d& position, const Color& color, double intensity, double range = 1000.0);

    const Vector3d& GetPosition() const;
    const Color& GetColor() const;
    double GetIntensity() const;
    double GetRange() const;

    void SetPosition(const Vector3d& position);
    void SetColor(const Color& color);
    void SetIntensity(double intensity);
    void SetRange(double range);

private:
    Vector3d position = Vector3d(0.0, 0.0, 100.0);
    Color color = Color(1.0, 1.0, 1.0);
    double intensity = 1.0;
    double range = 1000.0;
};
