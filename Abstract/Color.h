#pragma once

class Color
{
public:
    Color() = default;
    Color(int r, int g, int b);
    double GetR() const;
    double GetG() const;
    double GetB() const;

    void SetR(double r);
    void SetG(double g);
    void SetB(double b);

private:
    double r_;
    double g_;
    double b_;
};
