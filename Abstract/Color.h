#pragma once

class Color
{
public:
    Color() = default;
    Color(double r, double g, double b);

    double GetR() const;
    double GetG() const;
    double GetB() const;

    void SetR(double r);
    void SetG(double g);
    void SetB(double b);

    Color operator*(double k) const;
    Color operator*(const Color& other) const;
    Color operator+(const Color& other) const;

    Color Clamped() const;

private:
    double r_ = 0.0;
    double g_ = 0.0;
    double b_ = 0.0;
};
