#include <GL/glew.h>

#include "application/application.hpp"

#include "rendering/opengl/renderer.hpp"

#include "utilities/config.hpp"

Mosaic::OpenGLRenderer::OpenGLRenderer(Renderer& renderer)
    : mRenderer(renderer)
{
}

void Mosaic::OpenGLRenderer::Create()
{
    mContext = SDL_GL_CreateContext(mRenderer.mApplicationData.Window.mHandle);

    if (not mContext)
    {
        Console::Throw("Failed to create OpenGL context: {}", SDL_GetError());
    }

    SDL_GL_SetSwapInterval(mSwapInterval);

    glClearColor(mRenderer.mClearColour.X, mRenderer.mClearColour.Y, mRenderer.mClearColour.Z, mRenderer.mClearColour.W);

    if (glewInit())
    {
        Console::Throw("Failed to fetch OpenGL extensions: {}", SDL_GetError());
    }

    mRenderer.mApplicationData.EventManager.Subscribe(this, &OpenGLRenderer::OnResize);
}

void Mosaic::OpenGLRenderer::Update()
{
    glClearColor(mRenderer.mClearColour.X, mRenderer.mClearColour.Y, mRenderer.mClearColour.Z, mRenderer.mClearColour.W);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    SDL_GL_SwapWindow(mRenderer.mApplicationData.Window.mHandle);
}

void Mosaic::OpenGLRenderer::LoadConfig()
{
    TOMLFile file(mRenderer.mConfigPath);

    auto version = file.Get<uint32, 2>("OpenGL.Version");

    mVersion.X = version[0];
    mVersion.Y = version[1];

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, mVersion.X);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, mVersion.Y);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    switch (mRenderer.mVSync)
    {
        case (RendererVSync::Disabled):
        {
            mSwapInterval = 0;

            break;
        }
        case (RendererVSync::Strict):
        {
            mSwapInterval = 1;

            break;
        }
        case (RendererVSync::Relaxed):
        {
            mSwapInterval = -1;

            break;
        }
    }
}

void Mosaic::OpenGLRenderer::OnResize(const WindowResizeEvent& event)
{
    glViewport(0, 0, event.Size.X, event.Size.Y);
}