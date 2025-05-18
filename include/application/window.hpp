#pragma once

#include <SDL3/SDL.h>

#include "utilities/vector.hpp"

#include <cstdint>
#include <string>
#include <unordered_set>

namespace vk
{
    struct SurfaceKHR;
    struct Instance;
}

namespace Mosaic
{
    struct ApplicationData;

    struct WindowMoveEvent
    {
        Vector2<std::uint32_t> Position;
    };

    struct WindowResizeEvent
    {
        Vector2<std::uint32_t> Size;
    };

    class Window
    {
    public:
        Window(ApplicationData& applicationData);
        ~Window();

        Vector2<std::uint32_t> GetSize() const;
        void SetSize(const Vector2<std::uint32_t>& size);

        std::string GetTitle() const;
        void SetTitle(const std::string& title);

        std::string GetConfigPath() const;
        void SetConfigPath(const std::string& path);

        bool GetFullscreenState() const;
        void SetFullscreenState(bool fullscreen);

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

        Vector2<std::uint32_t> mSize;
        Vector2<std::uint32_t> mPosition;

        std::string mTitle;
        std::string mConfigPath;

        bool mRunning;
        bool mFullscreen;
        bool mResizable;

        SDL_Window* mHandle;

        float mDisplayScale;

        Vector2<std::uint32_t> mDisplayPhysicalResolution;

        ApplicationData* mApplicationData;

        friend class Application;
        friend class VulkanRenderer;
        friend class VulkanSurface;
        friend class VulkanInstance;
        friend class OpenGLRenderer;
    };
}