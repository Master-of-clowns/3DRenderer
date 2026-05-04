//
// Created by s24b_ on 04.05.2026.
//

#include "Screen.h"

Screen::Screen(ScreenWidth width, ScreenHeight height) : screen_(
    height, std::vector<Color>(width, Color(0.0, 0.0, 0.0))) {
}

int Screen::GetWidth() const {
    return screen_[0].size();
}

int Screen::GetHeight() const {
    return screen_.size();
}

void Screen::DrawPixel(int x, int y, Color&& color) {
    screen_[y][x] = std::move(color);
}

const Color& Screen::GetPixel(int x, int y) const {
    return screen_[y][x];
}



