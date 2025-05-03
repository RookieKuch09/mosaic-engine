#include "../../include/application/renderer.hpp"
#include "../../include/application/window.hpp"

Renderer::Renderer(RendererData& rendererData, WindowData& surfaceData)
    : mWindowData(&surfaceData), mRendererData(&rendererData)
{
}

Renderer::~Renderer()
{
}

void Renderer::Create()
{
    mRendererData->Context = SDL_GL_CreateContext(mWindowData->Handle);
    SDL_GL_MakeCurrent(mWindowData->Handle, mRendererData->Context);

    SDL_GL_SetSwapInterval(mRendererData->VSync);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        throw std::runtime_error("Failed to Load OpenGL extensions using GLEW");
    }

    glViewport(0, 0, mWindowData->Size.x, mWindowData->Size.y);
}

void Renderer::Update()
{
    OnResize();
    SwapBuffers();
}

void Renderer::OnResize()
{
    glViewport(0, 0, mWindowData->Size.x, mWindowData->Size.y);
}

void Renderer::SwapBuffers()
{
    SDL_GL_SwapWindow(mWindowData->Handle);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}