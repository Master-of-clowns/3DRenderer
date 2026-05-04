#pragma once

#include <SFML/Graphics.hpp>
#include "Screen.h"

class Display
{
public:
    Display(sf::RenderWindow* window, ScreenWidth width, ScreenHeight height);
    void Show(const Screen& screen);

private:
    sf::RenderWindow* window_;
    int width_;
    int height_;
};
