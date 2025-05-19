#pragma once

#include <SDL3/SDL.h>

#include "application/events.hpp"
#include "utilities/numerics.hpp"
#include "utilities/vector.hpp"

#include <string>
#include <unordered_set>

namespace vk
{
    struct SurfaceKHR;
    struct Instance;
}

namespace Mosaic::Internal
{
    class Application;
    class EventManager;
}

namespace Mosaic::Internal::Rendering
{
    class Renderer;
    class VulkanRenderer;
    class VulkanSurface;
    class VulkanInstance;
    class OpenGLRenderer;
}

namespace Mosaic::Internal::Windowing
{
    struct ApplicationData;

    struct WindowMoveEvent
    {
        Types::Vector2<Types::UInt32> Position;
    };

    struct WindowResizeEvent
    {
        Types::Vector2<Types::UInt32> Size;
    };

    class Window
    {
    public:
        Window(Rendering::Renderer& renderer, EventManager& eventManager);
        ~Window();

        void SetConfigPath(const std::string& path);

    private:
        void Create();
        void Update();

        void Initialise();
        void CreateWindow();

        void QuitEvent();
        void ResizeEvent(const SDL_Event& event);
        void MoveEvent(const SDL_Event& event);

        void LoadConfig();

        void InitialiseVulkan();
        void GetVulkanWindowSurface(vk::SurfaceKHR& surface, vk::Instance& instance) const;
        std::unordered_set<std::string> GetVulkanRequiredInstanceExtensions() const;

        Types::Vector2<Types::UInt32> mSize;
        Types::Vector2<Types::UInt32> mPosition;

        std::string mTitle;
        std::string mConfigPath;

        bool mRunning;
        bool mFullscreen;
        bool mResizable;

        SDL_Window* mHandle;

        Rendering::Renderer& mRenderer;
        EventManager& mEventManager;

        friend class Mosaic::Internal::Application;
        friend class Mosaic::Internal::Rendering::VulkanRenderer;
        friend class Mosaic::Internal::Rendering::VulkanSurface;
        friend class Mosaic::Internal::Rendering::VulkanInstance;
        friend class Mosaic::Internal::Rendering::OpenGLRenderer;
    };
}