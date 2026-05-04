//
// Created by s24b_ on 04.05.2026.
//

#include "Color.h"

Color::Color(int r, int g, int b) {
    r_ = r;
    g_ = g;
    b_ = b;
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


