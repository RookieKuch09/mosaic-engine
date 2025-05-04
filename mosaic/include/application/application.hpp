#pragma once

#include "components.hpp"
#include "events.hpp"
#include "input.hpp"
#include "logging.hpp"
#include "renderer.hpp"
#include "window.hpp"

namespace Mosaic
{
    struct ApplicationData
    {
        ApplicationData();

        Window Window;
        Renderer Renderer;

        ComponentManager ComponentManager;
        EventManager EventManager;
        InputManager InputManager;

        bool DebugMode = false;
    };

    class Application
    {
    public:
        virtual ~Application() = default;

        int Run();

    private:
        ApplicationData mData;
    };

    Application* CreateApplication();
}