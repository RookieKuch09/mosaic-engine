#pragma once

#include "components.hpp"
#include "console.hpp"
#include "events.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "window.hpp"

namespace Mosaic
{
    struct ApplicationData
    {
        ApplicationData();

        Window Window;
        Renderer Renderer;
        Console Console;

        ComponentManager ComponentManager;
        EventManager EventManager;
        InputManager InputManager;

        bool DebugMode;
    };

    class Application
    {
    public:
        virtual ~Application() = default;

        virtual void Setup();

        int Run();

    private:
        ApplicationData mData;
    };

    Application* CreateApplication();
}