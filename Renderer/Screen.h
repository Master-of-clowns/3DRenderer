#pragma once

#include "../Abstract/Color.h"
#include <vector>

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

private:
    std::vector<std::vector<Color>> screen_;
};
