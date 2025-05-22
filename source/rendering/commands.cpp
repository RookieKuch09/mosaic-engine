#include "rendering/commands.hpp"

namespace Mosaic::Internal::Rendering
{
    RendererCommandManager::RendererCommandManager(Renderer& renderer)
        : mRenderer(renderer)
    {
    }

    void RendererCommandManager::Submit(const RendererCommandWrapper& command)
    {
        mCommands.push_back(command);
    }

    void RendererCommandManager::Send()
    {
        // TODO: send commands to assigned Renderer
    }

    void RendererCommandManager::Purge()
    {
        mCommands.clear();
    }
}