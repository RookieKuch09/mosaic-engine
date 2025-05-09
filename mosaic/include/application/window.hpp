#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

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
        glm::uvec2 Position;
    };

    struct WindowResizeEvent
    {
        glm::uvec2 Size;
    };

    class Window
    {
    public:
        Window(ApplicationData& applicationData);
        ~Window();

        glm::uvec2 GetSize() const;
        void SetSize(const glm::uvec2& size);

        glm::uvec2 GetPosition() const;
        void SetPosition(const glm::uvec2& position);

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
        std::unordered_set<std::string> GetVulkanRequiredInstanceExtensions() const;
        void GetVulkanWindowSurface(vk::SurfaceKHR& surface, vk::Instance& instance) const;

        glm::uvec2 mSize;
        glm::uvec2 mPosition;

        std::string mTitle;
        std::string mConfigPath;

        bool mRunning;
        bool mFullscreen;
        bool mResizable;

        SDL_Window* mHandle;

        ApplicationData* mApplicationData;

        friend class Application;
        friend class VulkanRenderer;
        friend class VulkanSurface;
        friend class VulkanInstance;
        friend class OpenGLRenderer;
    };
}