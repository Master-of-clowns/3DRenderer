#include "Renderer.h"
#include <iostream>
#include <optional>

Renderer::Renderer(int width, int height)
	: width(width), height(height), camera(), world(){
	window.create(sf::VideoMode({
					static_cast<unsigned int>(width),
					static_cast<unsigned int>(height)
				}),
				"Renderer");
	window.setFramerateLimit(60);
}

void Renderer::SetCamera(const Camera& camera){
	this->camera = camera;
}

void Renderer::SetWorld(const World& world){
	this->world = world;
}

void Renderer::Render(){
	window.clear(backgroundColor);

	if (world.triangles.empty()){
		return;
	}

	for (const auto& triangle : world.triangles){
		DrawTriangle(triangle);
	}

	window.display();
}

sf::Vector2f Renderer::WorldToScreen(const Eigen::Vector3d& worldPoint) const{
	Eigen::Vector3d ndc = camera.WorldToScreen(worldPoint);

	float screenX = (static_cast<float>(ndc.x()) + 1.0f) * 0.5f * static_cast<float>(width);
	float screenY = (1.0f - static_cast<float>(ndc.y())) * 0.5f * static_cast<float>(height);

	return sf::Vector2f(screenX, screenY);
}

void Renderer::DrawTriangle(const Triangle& triangle){
	auto vertices = triangle.GetVertices();

	sf::Vector2f screenV1 = WorldToScreen(vertices[0]);
	sf::Vector2f screenV2 = WorldToScreen(vertices[1]);
	sf::Vector2f screenV3 = WorldToScreen(vertices[2]);

	std::array<sf::Vertex, 6> lines = {
		sf::Vertex(screenV1, lineColor),
		sf::Vertex(screenV2, lineColor),
		sf::Vertex(screenV2, lineColor),
		sf::Vertex(screenV3, lineColor),
		sf::Vertex(screenV3, lineColor),
		sf::Vertex(screenV1, lineColor)
	};

	window.draw(lines.data(), lines.size(), sf::PrimitiveType::Lines);
}

bool Renderer::IsOpen() const{
	return window.isOpen();
}

void Renderer::ProcessEvents(){
	while (const std::optional<sf::Event> event = window.pollEvent()){
		if (event->is<sf::Event::Closed>()){
			window.close();
		}

		if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()){
			if (keyEvent->scancode == sf::Keyboard::Scan::Escape){
				window.close();
			}
		}
	}
}

void Renderer::Close(){
	window.close();
}
