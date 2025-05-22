#pragma once

#include "application/components.hpp"
#include "application/console.hpp"
#include "application/window.hpp"
#include <Mosaic/include/application/application.hpp>
#include <Mosaic/include/application/console.hpp>

#include <Mosaic/include/application/events.hpp>
#include <Mosaic/include/utilities/config.hpp>
#include <Mosaic/include/utilities/vector.hpp>

#include <Mosaic/include/rendering/mesh.hpp>

namespace Mosaic
{
    using Internal::Component;
    using Internal::ComponentManager;
    using Internal::Console;
    using Internal::EventManager;
    using Internal::Instance;
    using Internal::Rendering::Mesh;
    using Internal::Rendering::Renderer;
    using Internal::Rendering::VertexAttribute;
    using Internal::Rendering::VertexFormat;
    using Internal::Windowing::Window;
    using Internal::Windowing::WindowMoveEvent;
    using Internal::Windowing::WindowResizeEvent;

    using namespace Internal::Types;
    using namespace Internal::Files;
}