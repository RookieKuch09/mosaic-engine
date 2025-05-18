#pragma once

#include "components.hpp"
#include "events.hpp"
#include "input.hpp"
#include "window.hpp"

#include "rendering/renderer.hpp"

namespace Mosaic
{
    enum class ApplicationPlatform
    {
        Windows,
        macOS,
        Linux,
    };

    struct ApplicationData
    {
        ApplicationData();

        Window Window;
        Renderer Renderer;

        ComponentManager ComponentManager;
        EventManager EventManager;
        InputManager InputManager;

        ApplicationPlatform Platform;

        bool DebugMode;
    };

    class Application
    {
    public:
        virtual ~Application() = default;

        virtual void Setup();

        std::int32_t Run();

        ApplicationData Data;
    };

    Application* CreateApplication();
}