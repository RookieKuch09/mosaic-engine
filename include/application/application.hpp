#pragma once

#include "components.hpp"
#include "events.hpp"
#include "input.hpp"
#include "window.hpp"

namespace Mosaic
{
    struct ApplicationData
    {
        ApplicationData();

        Window Window;

        ComponentManager ComponentManager;
        EventManager EventManager;
        InputManager InputManager;
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