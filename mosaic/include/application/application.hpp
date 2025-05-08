#pragma once

#include "components.hpp"
#include "console.hpp"
#include "events.hpp"
#include "input.hpp"
#include "renderer.hpp"
#include "window.hpp"

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
        Console Console;

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

        void SetRendererConfigPath(const std::string& path);

    private:
        ApplicationData mData;
    };

    Application* CreateApplication();
}