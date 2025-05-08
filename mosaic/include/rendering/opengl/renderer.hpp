#pragma once

#include "../../application/renderer.hpp"
#include "application/application.hpp"
#include "application/window.hpp"

#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_video.h>

#include <glm/glm.hpp>

namespace Mosaic
{
    class OpenGLRenderer : public RendererInterface
    {
    public:
        OpenGLRenderer(ApplicationData* applicationData);

    private:
        void Create() override;

        void PreUpdate() override;
        void PostUpdate() override;

        void LoadConfig() override;

        void OnResize(const WindowResizeEvent& event);

        SDL_GLContext mContext;

        glm::uvec2 mVersion;

        std::uint32_t mSwapInterval;

        ApplicationData* mApplicationData;
    };
}