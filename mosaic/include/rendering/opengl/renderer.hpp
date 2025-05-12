#pragma once

#include "../../application/application.hpp"
#include "../../application/renderer.hpp"
#include "../../application/window.hpp"

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_video.h>

namespace Mosaic
{
    class OpenGLRenderer : public RendererInterface
    {
    public:
        OpenGLRenderer(ApplicationData* applicationData);

    private:
        void Create() override;

        void Update() override;

        void LoadConfig() override;

        void OnResize(const WindowResizeEvent& event);

        SDL_GLContext mContext;

        Vector2<std::uint32_t> mVersion;

        std::uint32_t mSwapInterval;

        ApplicationData* mApplicationData;
    };
}