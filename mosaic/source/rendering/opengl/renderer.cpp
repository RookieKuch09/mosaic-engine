#include <GL/glew.h>

#include "../../../include/rendering/opengl/renderer.hpp"

#include "../../../include/utilities/config.hpp"
#include "application/application.hpp"
#include "application/renderer.hpp"
#include "application/window.hpp"

Mosaic::OpenGLRenderer::OpenGLRenderer(ApplicationData* applicationData)
    : mApplicationData(applicationData)
{
}

void Mosaic::OpenGLRenderer::Create()
{
    mContext = SDL_GL_CreateContext(mApplicationData->Window.mHandle);

    if (not mContext)
    {
        Console::Throw("Failed to create OpenGL context: {}", SDL_GetError());
    }

    SDL_GL_SetSwapInterval(mSwapInterval);

    glClearColor(mClearColour.x, mClearColour.y, mClearColour.z, mClearColour.a);

    if (glewInit())
    {
        Console::Throw("Failed to fetch OpenGL extensions: {}", SDL_GetError());
    }

    mApplicationData->EventManager.Subscribe(this, &OpenGLRenderer::OnResize);
}

void Mosaic::OpenGLRenderer::PreUpdate()
{
    glClearColor(mClearColour.x, mClearColour.y, mClearColour.z, mClearColour.a);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Mosaic::OpenGLRenderer::PostUpdate()
{
    SDL_GL_SwapWindow(mApplicationData->Window.mHandle);
}

void Mosaic::OpenGLRenderer::LoadConfig()
{
    TOMLFile file(mConfigPath);

    auto version = file.Get<std::uint32_t, 2>("OpenGL.Version");

    mVersion.x = version[0];
    mVersion.y = version[1];

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, mVersion.x);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, mVersion.y);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    switch (mVSync)
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
    glViewport(0, 0, event.Size.x, event.Size.y);
}