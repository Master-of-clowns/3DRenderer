#pragma once

#include "../Renderer/Renderer.h"
#include "../Renderer/Camera.h"
#include "../World/World.h"

class Application {
public:
    Application();
    void Run();

private:
    Renderer renderer_;
    Camera camera_;
    World world_;
};
