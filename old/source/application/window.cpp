#include "application/window.hpp"
#include "application/events.hpp"

#include "utilities/config.hpp"

#include "rendering/renderer.hpp"

#include <SDL3/SDL_vulkan.h>

#include <vulkan/vulkan.hpp>

namespace Mosaic::Internal::Windowing
{
    Window::Window(Rendering::Renderer& renderer, EventManager& eventManager)
        : mSize(0, 0), mPosition(0, 0), mTitle(""), mConfigPath(""), mRunning(false), mFullscreen(false), mRenderer(renderer), mEventManager(eventManager)
    {
        Initialise();
    }

    Window::~Window()
    {
        SDL_DestroyWindow(mHandle);
        SDL_Quit();
    }

    void Window::SetConfigPath(const std::string& path)
    {
        mConfigPath = path;
    }

    void Window::Create()
    {
        CreateWindow();

        mRunning = true;
    }

    void Window::Update()
    {
        SDL_PumpEvents();

        SDL_Event event;

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                QuitEvent();
            }
            else if (event.window.type == SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED)
            {
                ResizeEvent(event);
            }
            else if (event.window.type == SDL_EVENT_WINDOW_MOVED)
            {
                MoveEvent(event);
            }
        }
    }

    void Window::Initialise()
    {
        if (not SDL_Init(SDL_INIT_VIDEO))
        {
            Console::Throw("Failed to initialise windowing system");
        }
    }

    void Window::CreateWindow()
    {
        Types::UI32 flags = 0;

        switch (mRenderer.mAPI)
        {
            case (Rendering::RendererAPI::Vulkan):
            {
                flags |= SDL_WINDOW_VULKAN;

                break;
            }
            case (Rendering::RendererAPI::OpenGL):
            {
                flags |= SDL_WINDOW_OPENGL;

                break;
            }
        }

        if (mResizable)
        {
            flags |= SDL_WINDOW_RESIZABLE;
        }

        mHandle = SDL_CreateWindow(mTitle.c_str(), mSize.X, mSize.Y, flags);

        if (not mHandle)
        {
            Console::Throw("Failed to create window");
        }

        SDL_SetWindowFullscreen(mHandle, mFullscreen);
    }

    void Window::QuitEvent()
    {
        mRunning = false;
    }

    void Window::ResizeEvent(const SDL_Event& event)
    {
        mSize.X = event.window.data1;
        mSize.Y = event.window.data2;

        mEventManager.Emit<WindowResizeEvent>({mSize});
    }

    void Window::MoveEvent(const SDL_Event& event)
    {
        mPosition.X = event.window.data1;
        mPosition.Y = event.window.data2;

        mEventManager.Emit<WindowMoveEvent>({mPosition});
    }

    void Window::LoadConfig()
    {
        Files::TOMLFile file;

        file.Open(mConfigPath);

        mTitle = file.Get<std::string>("Window.Title");
        mFullscreen = file.Get<bool>("Window.Fullscreen", false);
        mResizable = file.Get<bool>("Window.Resizable", false);
        auto size = file.Get<Types::UI32, 2>("Window.Size");

        mSize.X = size[0];
        mSize.Y = size[1];
    }

    void Window::InitialiseVulkan()
    {
        if (not SDL_Vulkan_LoadLibrary(nullptr))
        {
            Console::Throw("Vulkan initialisation failed");
        }
    }

    std::unordered_set<std::string> Window::GetVulkanRequiredInstanceExtensions() const
    {
        Types::UI32 count = 0;

        const char* const* raw = SDL_Vulkan_GetInstanceExtensions(&count);

        std::unordered_set<std::string> extensions;

        for (Types::UI32 index = 0; index < count; index++)
        {
            extensions.insert(raw[index]);
        }

        return extensions;
    }

    void Window::GetVulkanWindowSurface(vk::SurfaceKHR& surface, vk::Instance& instance) const
    {
        if (not SDL_Vulkan_CreateSurface(mHandle, instance, nullptr, reinterpret_cast<VkSurfaceKHR*>(&surface)))
        {
            Console::Throw("Failed to create vk::UniqueSurfaceKHR");
        }
    }
}