#include "utilities/rect.hpp"

#include "rendering/target.hpp"

namespace Mosaic::Internal::Rendering
{
    RenderTarget::RenderTarget(Types::Rectangle&& area, RendererCommandManager& commands)
        : mArea(area), mRendererCommandManager(commands)
    {
    }

    void RenderTarget::Render()
    {
        // TODO: submit viewport resize command

        // TODO: run render graph
    }
}