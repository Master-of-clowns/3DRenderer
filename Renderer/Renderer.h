#ifndef RENDERER_H
#define RENDERER_H

#include "../Camera/Camera.h"
#include "../World/World.h"
#include <SFML/Graphics.hpp>

class Renderer {
private:
	int width;
	int height;
	Camera camera;
	World world;

	sf::RenderWindow window;

	sf::Color backgroundColor = sf::Color::Black;
	sf::Color lineColor = sf::Color::White;

public:
	Renderer(int width, int height);

	void SetCamera(const Camera& camera);
	void SetWorld(const World& world);

	void Render();
	bool IsOpen() const;
	void ProcessEvents();
	void Close();

private:
	sf::Vector2f WorldToScreen(const Eigen::Vector3d& worldPoint) const;
	void DrawTriangle(const Triangle& triangle);
};

#endif // RENDERER_H