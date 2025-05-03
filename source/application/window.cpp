#include "../../include/application/window.hpp"
#include "../../include/application/renderer.hpp"

Window::Window(WindowData& surfaceData, RendererData& rendererData)
    : mWindowData(&surfaceData), mRendererData(&rendererData)
{
    Initialise();
}
Window::~Window()
{
    SDL_DestroyWindow(mWindowData->Handle);
    SDL_Quit();
}

void Window::Create()
{
    CreateWindow();

    mWindowData->Running = true;
}

void Window::Update()
{
    SDL_PumpEvents();

    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            QuitEvent();
        }
        else if (event.type == SDL_WINDOWEVENT and event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
        {
            ResizeEvent(event);
        }
    }
}

void Window::Initialise()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        throw std::runtime_error("Failed to initialise SDL2 video");
    }
}

void Window::CreateWindow()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, mRendererData->Version[0]);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, mRendererData->Version[1]);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    unsigned int flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE;

    mWindowData->Handle = SDL_CreateWindow(mWindowData->Title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mWindowData->Size.x, mWindowData->Size.y, flags);

    if (!mWindowData->Handle)
    {
        throw std::runtime_error("Failed to create SDL2 window");
    }
}

void Window::QuitEvent()
{
    mWindowData->Running = false;
}

void Window::ResizeEvent(const SDL_Event& event)
{
    if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
    {
        mWindowData->Size.x = event.window.data1;
        mWindowData->Size.y = event.window.data2;
    }
}