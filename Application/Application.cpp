#include "Application.h"
#include "../ParserOBJ/ParserOBJ.h"

#include <iostream>

// Я понимаю, что все эти параметры нужно вынести в отдельный файл, но пока сделал так.

Application::Application()
    : window_(sf::VideoMode({1800, 1800}), "3D Renderer")
      , screen_(ScreenWidth{static_cast<int>(1800)}, ScreenHeight{static_cast<int>(1800)})
      , display_(&window_, ScreenWidth{static_cast<int>(1800)}, ScreenHeight{static_cast<int>(1800)})
      , camera_(
          Vector3d(0.0, -40.0, 10.0),
          Vector3d(0.0, 1.0, 0.0),
          Vector3d(0.0, 0.0, 1.0),
          60.0,
          static_cast<double>(1800) / 1800,
          0.1,
          2000.0
          ) {
    window_.setFramerateLimit(60);

    ParserOBJ parser;
    std::string filename = "../12140_Skull_v3_L2.obj";

    std::cout << "Loading OBJ file: " << filename << std::endl;
    world_ = World(parser.Parse(filename));
}


void Application::Run() {
    while (window_.isOpen()) {
        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window_.close();
            }
        }

        window_.clear(sf::Color::Black);
        display_.Show(renderer_.Render(world_, camera_, screen_));
        window_.display();
    }
}
