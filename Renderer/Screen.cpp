#include "Screen.h"
#include <utility>

Screen::Screen(ScreenWidth width, ScreenHeight height)
    : screen_(height, std::vector<Color>(width, Color(0.0, 0.0, 0.0))),
      depth_(height, std::vector<double>(width, std::numeric_limits<double>::infinity())) {
}

int Screen::GetWidth() const {
    return screen_.empty() ? 0 : static_cast<int>(screen_[0].size());
}

int Screen::GetHeight() const {
    return static_cast<int>(screen_.size());
}

bool Screen::InBounds(int x, int y) const {
    return x >= 0 && x < GetWidth() && y >= 0 && y < GetHeight();
}

void Screen::DrawPixel(int x, int y, Color&& color) {
    if (InBounds(x, y)) {
        screen_[y][x] = std::move(color);
    }
}

const Color& Screen::GetPixel(int x, int y) const {
    return screen_[y][x];
}

void Screen::Clear(const Color& background) {
    for (int y = 0; y < GetHeight(); ++y) {
        for (int x = 0; x < GetWidth(); ++x) {
            screen_[y][x] = background;
            depth_[y][x] = std::numeric_limits<double>::infinity();
        }
    }
}

bool Screen::DrawPixelDepth(int x, int y, double depth, const Color& color) {
    if (!InBounds(x, y)) {
        return false;
    }

    if (depth < depth_[y][x]) {
        depth_[y][x] = depth;
        screen_[y][x] = color;
        return true;
    }
    return false;
}
