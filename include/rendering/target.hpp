#pragma once

#include "rendering/commands.hpp"
#include "utilities/rect.hpp"

namespace Mosaic::Internal::Windowing
{
    class Window;
}

namespace Mosaic::Internal::Rendering
{
    class RendererCommandManager;
}

namespace Mosaic::Internal::Rendering
{
    class RenderTarget
    {
    public:
        RenderTarget(Types::Rectangle&& area, RendererCommandManager& renderer);

    private:
        void Render();

        Types::Rectangle mArea;

        RendererCommandManager& mRendererCommandManager;
    };
}