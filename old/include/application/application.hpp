#pragma once

#include "components.hpp"
#include "events.hpp"
#include "input.hpp"
#include "window.hpp"

#include "rendering/renderer.hpp"

namespace Mosaic::Internal
{
    enum class ApplicationPlatform
    {
        Windows,
        macOS,
        Linux,
    };

    class Instance
    {
    public:
        Instance(ComponentManager& componentManager, EventManager& eventManager, Windowing::Window& window, Rendering::Renderer& renderer);

        static Instance* ProvideInstance(ComponentManager& componentManager, EventManager& eventManager, Windowing::Window& window, Rendering::Renderer& renderer);

    protected:
        Windowing::Window& mWindow;
        Rendering::Renderer& mRenderer;

        EventManager& mEventManager;
        ComponentManager& mComponentManager;

        friend class Application;
    };

    class Application
    {
    public:
        Application();

        Types::I32 Run();

        Windowing::Window mWindow;
        Rendering::Renderer mRenderer;

        ComponentManager mComponentManager;
        EventManager mEventManager;
        InputManager mInputManager;

        ApplicationPlatform mPlatform;

        Instance* mInstance;

        bool mDebugMode;
    };
}