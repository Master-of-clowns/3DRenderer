using namespace std;

#include <SFML/Graphics.hpp>

#include "ParserOBJ/ParserOBJ.h"
#include "Figures/Triangle.h"
#include "World/World.h"
#include "Renderer/Renderer.h"
#include "Renderer/Camera.h"

int main() {
    const int width = 1800;
    const int height = 1800;

    Renderer renderer(width, height);

    ParserOBJ parser;
    std::string filename = "../12140_Skull_v3_L2.obj";

    std::cout << "Loading OBJ file: " << filename << std::endl;

    try {
        World world(parser.Parse(filename));
        // std::cout << "File loaded successfully!" << std::endl;
        // std::cout << "Triangles loaded: " << world.triangles.size() << std::endl;

        renderer.SetWorld(world);

        Camera camera;
        // camera.position = Eigen::Vector3d(0, 0, 5);  // Камера на расстоянии 5 единиц
        // camera.target = Eigen::Vector3d(0, 0, 0);    // Смотрим в центр
        // camera.up = Eigen::Vector3d(0, 1, 0);        // Верх - ось Y
        // camera.fov = 60.0;                           // Поле зрения 60 градусов
        // camera.aspectRatio = static_cast<double>(width) / height;
        // camera.nearPlane = 0.1;
        // camera.farPlane = 100.0;

        // camera.UpdateViewMatrix();
        // camera.UpdateProjectionMatrix();

        // Устанавливаем камеру в рендерер
        renderer.SetCamera(camera);

        std::cout << "Camera configured. Press ESC to exit." << std::endl;

        while (renderer.IsOpen()) {
            renderer.ProcessEvents();

            renderer.Render();
        }

    } catch (const std::exception& e) {
        std::cerr << "Error loading OBJ file: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "Renderer closed." << std::endl;
    return 0;
}
