#pragma once

#include "camera.hpp"
#include "components.hpp"
#include "events.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "window.hpp"

struct ApplicationData
{
    // Standalone objects
    Window Window;
    Renderer Renderer;
    Camera Camera;

    // Standalone object data
    WindowData WindowData;
    RendererData RendererData;
    CameraData CameraData;

    // System managers
    ComponentManager ComponentManager;
    EventManager EventManager;
    InputManager InputManager;
};

class Application
{
public:
    Application();

private:
    void LoadSettings();
    int Run();

    ApplicationData mApplicationData;

    friend int main();
};

Application* CreateApplication();