#include "../../include/application/application.hpp"

#include "../../include/utilities/config.hpp"

#include <SDL3/SDL_vulkan.h>

#include <vulkan/vulkan.hpp>

Mosaic::Window::Window(ApplicationData& applicationData)
    : mSize(0, 0), mPosition(0, 0), mTitle(""), mConfigPath(""), mRunning(false), mFullscreen(false), mApplicationData(&applicationData)
{
    Initialise();
}

Mosaic::Window::~Window()
{
    SDL_DestroyWindow(mHandle);
    SDL_Quit();
}

Mosaic::Vector2<std::uint32_t> Mosaic::Window::GetSize() const
{
    return mSize;
}

std::string Mosaic::Window::GetTitle() const
{
    return mTitle;
}

void Mosaic::Window::SetSize(const Vector2<std::uint32_t>& size)
{
    mSize = size;

    SDL_SetWindowSize(mHandle, mSize.X, mSize.X);
}

void Mosaic::Window::SetTitle(const std::string& title)
{
    mTitle = title;

    SDL_SetWindowTitle(mHandle, title.c_str());
}

std::string Mosaic::Window::GetConfigPath() const
{
    return mConfigPath;
}

void Mosaic::Window::SetConfigPath(const std::string& path)
{
    mConfigPath = path;
}

bool Mosaic::Window::GetFullscreenState() const
{
    return mFullscreen;
}

void Mosaic::Window::SetFullscreenState(bool fullscreen)
{
    mFullscreen = fullscreen;

    SDL_SetWindowFullscreen(mHandle, mFullscreen);
}

void Mosaic::Window::Create()
{
    CreateWindow();

    mRunning = true;
}

void Mosaic::Window::Update()
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

void Mosaic::Window::Initialise()
{
    if (not SDL_Init(SDL_INIT_VIDEO))
    {
        Console::Throw("Failed to initialise windowing system");
    }
}

void Mosaic::Window::CreateWindow()
{
    std::uint64_t flags = 0;

    switch (mApplicationData->Renderer.GetRendererAPI())
    {
        case (RendererAPI::Vulkan):
        {
            flags |= SDL_WINDOW_VULKAN;

            break;
        }
        case (RendererAPI::OpenGL):
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

void Mosaic::Window::QuitEvent()
{
    mRunning = false;
}

void Mosaic::Window::ResizeEvent(const SDL_Event& event)
{
    mSize.X = event.window.data1;
    mSize.Y = event.window.data2;

    mApplicationData->EventManager.Emit<WindowResizeEvent>({mSize});
}

void Mosaic::Window::MoveEvent(const SDL_Event& event)
{
    mPosition.X = event.window.data1;
    mPosition.Y = event.window.data2;

    mApplicationData->EventManager.Emit<WindowMoveEvent>({mPosition});
}

void Mosaic::Window::LoadConfig()
{
    TOMLFile file;

    file.Open(mConfigPath);

    mTitle = file.Get<std::string>("Window.Title");
    mFullscreen = file.Get<bool>("Window.Fullscreen", false);
    mResizable = file.Get<bool>("Window.Resizable", false);
    auto size = file.Get<std::uint32_t, 2>("Window.Size");

    mSize.X = size[0];
    mSize.Y = size[1];
}

void Mosaic::Window::InitialiseVulkan()
{
    if (not SDL_Vulkan_LoadLibrary(nullptr))
    {
        Console::Throw("Vulkan initialisation failed");
    }
}

std::unordered_set<std::string> Mosaic::Window::GetVulkanRequiredInstanceExtensions() const
{
    std::uint32_t count = 0;

    const char* const* raw = SDL_Vulkan_GetInstanceExtensions(&count);

    std::unordered_set<std::string> extensions;

    for (std::uint32_t index = 0; index < count; index++)
    {
        extensions.insert(raw[index]);
    }

    return extensions;
}

void Mosaic::Window::GetVulkanWindowSurface(vk::SurfaceKHR& surface, vk::Instance& instance) const
{
    if (not SDL_Vulkan_CreateSurface(mHandle, instance, nullptr, reinterpret_cast<VkSurfaceKHR*>(&surface)))
    {
        Console::Throw("Failed to create vk::UniqueSurfaceKHR");
    }
}