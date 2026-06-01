#include "Application.h"
#include "../ParserOBJ/ParserOBJ.h"
#include "../Figures/Object.h"
#include "../Renderer/Light.h"

#include <iostream>

namespace {
constexpr unsigned int kWidth  = 600;
constexpr unsigned int kHeight = 600;
}

Application::Application()
    : window_(sf::VideoMode({kWidth, kHeight}), "3D Renderer")
    , screen_(ScreenWidth{static_cast<int>(kWidth)}, ScreenHeight{static_cast<int>(kHeight)})
    , display_(&window_, ScreenWidth{static_cast<int>(kWidth)}, ScreenHeight{static_cast<int>(kHeight)})
    , camera_(
        Vector3d(-100.0, 0, 0.0),
        Vector3d(-1.0, 0, 0.0),
        Vector3d(0.0, 0.0, 1.0),
        60.0,
        static_cast<double>(kWidth) / kHeight,
        0.1,
        2000.0
      )
    , camera_controller_(&camera_, Vector3d(0.0, 0.0, 0.0), 200.0, 0.0, 0.3)
{
    window_.setFramerateLimit(60);

    ParserOBJ parser;
    std::string filename = "../humster.obj";
    std::cout << "Loading OBJ file: " << filename << std::endl;

    Object skull(parser.Parse(filename), Vector3d(0.0, 0.0, .0));
    std::cout << "len " << skull.GetTriangles().size() << std::endl;
    skull.SetColor(Color(0.85, 0.80, 0.70));
    world_.AddObject(std::move(skull));

    Light light(
        Vector3d(150.0, 150.0, 200.0),
        Color(1.0, 1.0, 1.0),
        1.0,
        400.0
    );
    world_.SetLight(light);
}

void Application::Run() {
    sf::Clock clock;

    double secondAccumulator = 0.0;
    int frames = 0;

    while (window_.isOpen()) {
        double deltaSeconds = clock.restart().asSeconds();

        while (const std::optional event = window_.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window_.close();
            } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->code == sf::Keyboard::Key::Tab) {
                    renderer_.ToggleRenderMode();
                }
            }
        }

        camera_controller_.Update(deltaSeconds);

        window_.clear(sf::Color::Black);
        display_.Show(renderer_.Render(world_, camera_, screen_));
        window_.display();

        ++frames;
        secondAccumulator += deltaSeconds;
        if (secondAccumulator >= 1.0) {
            std::cout << "FPS: " << frames << std::endl;
            frames = 0;
            secondAccumulator -= 1.0;
        }
    }
}