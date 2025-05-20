#pragma once

#include "rendering/renderer.hpp"

#include "utilities/numerics.hpp"

#include <SDL3/SDL_video.h>

namespace Mosaic::Internal::Windowing
{
    struct WindowResizeEvent;
}

namespace Mosaic::Internal::Rendering
{
    class OpenGLRenderer : public RendererInterface
    {
    public:
        OpenGLRenderer(Renderer& renderer);

    private:
        void Create() override;
        void Update() override;
        void LoadConfig() override;

        void OnResize(const Windowing::WindowResizeEvent& event);

        SDL_GLContext mContext;

        Types::UI32 mSwapInterval;
        Types::Vector2<Types::UI32> mVersion;
        Renderer& mRenderer;
    };
}