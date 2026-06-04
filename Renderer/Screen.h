#pragma once

#include "../Abstract/Color.h"
#include <vector>
#include <limits>

enum ScreenWidth : int;
enum ScreenHeight : int;

class Screen
{
public:
    Screen(ScreenWidth width, ScreenHeight height);

    int GetWidth() const;
    int GetHeight() const;

    void DrawPixel(int x, int y, Color&& color);
    const Color& GetPixel(int x, int y) const;

    void Clear(const Color& background);

    bool DrawPixelDepth(int x, int y, double depth, const Color& color);

private:
    bool InBounds(int x, int y) const;

    std::vector<std::vector<Color>> screen_;
    std::vector<std::vector<double>> depth_;
};
