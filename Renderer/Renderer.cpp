#include "Renderer.h"
#include <iostream>
#include <optional>

Renderer::Renderer(int width, int height) :
    width_(width), height_(height) {
    window_.create(sf::VideoMode({
                       static_cast<unsigned int>(width),
                       static_cast<unsigned int>(height)
                   }),
                   "Renderer");
    window_.setFramerateLimit(60);
}

void Renderer::SetCamera(const Camera& camera) {
    camera_ = camera;
}

void Renderer::SetWorld(const World& world) {
    world_ = world;
}

void Renderer::Render() {
    window_.clear(backgroundColor_);

    if (world_.IsEmpty()) {
        return;
    }

    for (const auto& triangle : world_) {
        DrawTriangle(triangle);
    }

    window_.display();
}

bool Renderer::IsOpen() const {
    return window_.isOpen();
}

void Renderer::ProcessEvents() {
    while (const std::optional<sf::Event> event = window_.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window_.close();
        }

        if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
            if (keyEvent->scancode == sf::Keyboard::Scan::Escape) {
                window_.close();
            }
        }
    }
}

void Renderer::Close() {
    window_.close();
}

sf::Vector2f Renderer::WorldToScreen(const Eigen::Vector3d& worldPoint) {
    Eigen::Vector3d ndc = camera_.WorldToScreen(worldPoint);

    float screenX = (static_cast<float>(ndc.x()) + 1.0f) * 0.5f * static_cast<float>(width_);
    float screenY = (1.0f - static_cast<float>(ndc.y())) * 0.5f * static_cast<float>(height_);

    return sf::Vector2f(screenX, screenY);
}

void Renderer::DrawTriangle(const Triangle& triangle) {
    auto vertices = triangle.GetVertices();

    sf::Vector2f screenV1 = WorldToScreen(vertices[0]);
    sf::Vector2f screenV2 = WorldToScreen(vertices[1]);
    sf::Vector2f screenV3 = WorldToScreen(vertices[2]);

    std::array<sf::Vertex, 6> lines = {
        sf::Vertex(screenV1, lineColor_),
        sf::Vertex(screenV2, lineColor_),
        sf::Vertex(screenV2, lineColor_),
        sf::Vertex(screenV3, lineColor_),
        sf::Vertex(screenV3, lineColor_),
        sf::Vertex(screenV1, lineColor_)
    };

    window_.draw(lines.data(), lines.size(), sf::PrimitiveType::Lines);
}
