#pragma once

#include <Mosaic/include/application/application.hpp>
#include <Mosaic/include/application/console.hpp>
#include <Mosaic/include/application/main.hpp>

namespace Mosaic
{
    using Internal::Component;
    using Internal::ComponentManager;
    using Internal::Console;
    using Internal::EventManager;
    using Internal::Instance;
    using Internal::Rendering::Renderer;
    using Internal::Windowing::Window;
    using Internal::Windowing::WindowMoveEvent;
    using Internal::Windowing::WindowResizeEvent;

    using namespace Internal::Types;
}