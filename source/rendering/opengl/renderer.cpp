#include <GL/glew.h>

#include "application/console.hpp"
#include "application/window.hpp"

#include "rendering/opengl/renderer.hpp"

namespace Mosaic::Internal::Rendering
{
    OpenGLRenderer::OpenGLRenderer(Renderer& renderer)
        : mRenderer(renderer)
    {
    }

    void OpenGLRenderer::Create()
    {
        mContext = SDL_GL_CreateContext(mRenderer.mWindow.mHandle);

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

        mRenderer.mEventManager.Subscribe(this, &OpenGLRenderer::OnResize);
    }

    void OpenGLRenderer::Update()
    {
        glClearColor(mRenderer.mClearColour.X, mRenderer.mClearColour.Y, mRenderer.mClearColour.Z, mRenderer.mClearColour.W);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        SDL_GL_SwapWindow(mRenderer.mWindow.mHandle);
    }

    void OpenGLRenderer::LoadConfig()
    {
        if (not GLEW_VERSION_4_6)
        {
            Console::Throw("System does not support required OpenGL version (Mosaic requires 4.6 Core)");
        }

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
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

    void OpenGLRenderer::OnResize(const Windowing::WindowResizeEvent& event)
    {
        glViewport(0, 0, event.Size.X, event.Size.Y);
    }
}