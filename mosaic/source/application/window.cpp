#include "../../include/application/application.hpp"

#include "../../include/utilities/config.hpp"

#include <SDL3/SDL_video.h>

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

glm::uvec2 Mosaic::Window::GetSize() const
{
    return mSize;
}

glm::uvec2 Mosaic::Window::GetPosition() const
{
    return mPosition;
}

std::string Mosaic::Window::GetTitle() const
{
    return mTitle;
}

void Mosaic::Window::SetSize(const glm::uvec2& size)
{
    mSize = size;

    SDL_SetWindowSize(mHandle, mSize.x, mSize.y);
}

void Mosaic::Window::SetPosition(const glm::uvec2& position)
{
    mPosition = position;

    SDL_SetWindowPosition(mHandle, mPosition.x, mPosition.y);
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
        Console::LogError("Failed to initialise windowing system");
    }
}

void Mosaic::Window::CreateWindow()
{
    std::uint32_t flags = 0;

    switch (mApplicationData->Renderer.GetRendererAPI())
    {
        case (RendererAPI::Vulkan):
        {
            flags = SDL_WINDOW_VULKAN;
        }
        case (RendererAPI::OpenGL):
        {
            flags = SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE;
        }
    }

    mHandle = SDL_CreateWindow(mTitle.c_str(), mSize.x, mSize.y, flags);

    if (not mHandle)
    {
        Console::LogError("Failed to create window");
    }

    SDL_SetWindowFullscreen(mHandle, mFullscreen);
}

void Mosaic::Window::QuitEvent()
{
    mRunning = false;
}

void Mosaic::Window::ResizeEvent(const SDL_Event& event)
{
    mSize.x = event.window.data1;
    mSize.y = event.window.data2;

    mApplicationData->EventManager.Emit<WindowResizeEvent>({mSize});
}

void Mosaic::Window::MoveEvent(const SDL_Event& event)
{
    mPosition.x = event.window.data1;
    mPosition.y = event.window.data2;

    mApplicationData->EventManager.Emit<WindowMoveEvent>({mPosition});
}

void Mosaic::Window::LoadConfig()
{
    TOMLFile file;

    file.Open(mConfigPath);

    mTitle = file.Get<std::string>("window.title", "New Window - Mosaic");
    mFullscreen = file.Get<bool>("window.fullscreen", false);
    auto size = file.Get<std::uint32_t, 2>("window.size", {800, 600});

    mSize.x = size[0];
    mSize.y = size[1];
}