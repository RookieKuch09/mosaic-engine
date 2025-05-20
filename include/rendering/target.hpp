#pragma once

#include "utilities/rect.hpp"

namespace Mosaic::Internal::Windowing
{
    class Window;
}

namespace Mosaic::Internal::Rendering
{
    class Renderer;
}

namespace Mosaic::Internal::Rendering
{
    class RenderTarget
    {
    public:
        RenderTarget(Windowing::Window& window, Rendering::Renderer& renderer, const Types::Rectangle& rect);

    private:
        Types::Rectangle mArea;

        Windowing::Window& mWindow;
        Rendering::Renderer& mRenderer;
    };
}