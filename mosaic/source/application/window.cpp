#include "../../include/application/window.hpp"
#include "../../include/application/application.hpp"
#include "../../include/application/logging.hpp"

Mosaic::Window::Window(ApplicationData& applicationData)
    : mApplicationData(&applicationData), mSize(0, 0), mPosition(0, 0), mRunning(false)
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
        LogError("Failed to initialise windowing system");
    }
}

void Mosaic::Window::CreateWindow()
{
    unsigned int flags = SDL_WINDOW_RESIZABLE | SDL_WINDOW_VULKAN;

    mHandle = SDL_CreateWindow(mTitle.c_str(), mSize.x, mSize.y, flags);

    if (not mHandle)
    {
        LogError("Failed to create window");
    }
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