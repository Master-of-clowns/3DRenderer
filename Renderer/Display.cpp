#include "Display.h"
#include <cstdint>

Display::Display(sf::RenderWindow* window, ScreenWidth width, ScreenHeight height)
    : window_(window)
      , width_(static_cast<int>(width))
      , height_(static_cast<int>(height)) {
}

void Display::Show(const Screen& screen) {
    sf::Image image(sf::Vector2u(width_, height_), sf::Color::Black);

    for (int y = 0; y < height_; ++y) {
        for (int x = 0; x < width_; ++x) {
            const Color& color = screen.GetPixel(x, y);
            sf::Color sfmlColor(
                static_cast<std::uint8_t>(color.GetR() * 255),
                static_cast<std::uint8_t>(color.GetG() * 255),
                static_cast<std::uint8_t>(color.GetB() * 255)
                );
            image.setPixel(sf::Vector2u(x, y), sfmlColor);
        }
    }

    sf::Texture texture(image);
    sf::Sprite sprite(texture);
    window_->draw(sprite);
}
