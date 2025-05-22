#pragma once

#include "rendering/commands.hpp"

#include "utilities/numerics.hpp"
#include "utilities/vector.hpp"

#include <string>

namespace Mosaic::Internal
{
    class Application;
    class EventManager;
}

namespace Mosaic::Internal::Windowing
{
    class Window;
}

namespace Mosaic::Internal::Rendering
{
    enum class RendererVSync
    {
        Disabled,
        Strict,
        Relaxed,
    };

    enum class RendererAPI
    {
        Vulkan,
        OpenGL,
    };

    class RendererInterface
    {
    protected:
        virtual void LoadConfig() = 0;
        virtual void Create() = 0;
        virtual void Update() = 0;

        friend class Renderer;
    };

    class Renderer
    {
    public:
        Renderer(Windowing::Window& window, EventManager& eventManager);

        void SetConfigPath(const std::string& path);

    protected:
        void LoadConfig();
        void Create();
        void Update();
        virtual void ManageCommands() = 0;

        std::string mConfigPath;

        Types::Vec4<Types::F32> mClearColour;

        RendererAPI mAPI;
        RendererVSync mVSync;
        RendererInterface* mBackend;

        Windowing::Window& mWindow;
        EventManager& mEventManager;

        friend class Mosaic::Internal::Application;
        friend class Mosaic::Internal::Windowing::Window;
        friend class OpenGLRenderer;
        friend class VulkanRenderer;
    };
}