#pragma once

#include "../Renderer/Renderer.h"
#include "../Renderer/Camera.h"
#include "../World/World.h"
#include "../Renderer/Display.h"
#include "../Renderer/Screen.h"

#include <SFML/Graphics.hpp>

class Application {
public:
    Application();
    void Run();

private:
    sf::RenderWindow window_;
    Screen screen_;
    Display display_;

    Camera camera_;
    World world_;
    Renderer renderer_;
};