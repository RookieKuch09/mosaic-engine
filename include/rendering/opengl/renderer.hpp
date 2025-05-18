#pragma once

#include "application/window.hpp"
#include "rendering/renderer.hpp"

#include "utilities/arithmetic.hpp"

namespace Mosaic
{
    class OpenGLRenderer : public RendererInterface
    {
    public:
        OpenGLRenderer(Renderer& renderer);

    private:
        void Create() override;
        void Update() override;
        void LoadConfig() override;

        void OnResize(const WindowResizeEvent& event);

        SDL_GLContext mContext;

        uint32 mSwapInterval;
        Vector2<uint32> mVersion;
        Renderer& mRenderer;
    };
}