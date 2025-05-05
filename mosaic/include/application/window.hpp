#pragma once

#include <SDL3/SDL.h>
#include <glm/glm.hpp>

#include <string>

static constexpr std::uint64_t FPS_INTERVAL_MS = 1000;

namespace Mosaic
{
    class ApplicationData;

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
        glm::uvec2 GetPosition() const;
        std::string GetTitle() const;

        void SetSize(const glm::uvec2& size);
        void SetPosition(const glm::uvec2& position);
        void SetTitle(const std::string& title);

    private:
        void Create();
        void Update();

        void Initialise();
        void CreateWindow();

        void QuitEvent();
        void ResizeEvent(const SDL_Event& event);
        void MoveEvent(const SDL_Event& event);

        glm::uvec2 mSize;
        glm::uvec2 mPosition;

        std::string mTitle;

        bool mRunning;

        SDL_Window* mHandle;

        ApplicationData* mApplicationData;

        std::uint64_t mFrameCounter = 0;
        std::uint64_t mLastTime = 0;

        friend class Application;
        friend class Renderer;
    };
}