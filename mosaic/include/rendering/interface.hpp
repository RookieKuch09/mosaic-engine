#pragma once

#include <glm/glm.hpp>

namespace Mosaic
{
    enum class VSyncMode
    {
        Disabled,
        StrictVSync,
        RelaxedVSync,
    };

    class RendererInterface
    {
    public:
        virtual ~RendererInterface() = default;

        glm::fvec4 GetSwapColour() const;
        void SetSwapColour(const glm::fvec4& colour);

        VSyncMode GetVSyncMode() const;
        void SetVSyncMode(VSyncMode& mode);

    protected:
        VSyncMode mVSyncMode;
        glm::fvec4 mSwapColour;

        virtual void Create() = 0;

        virtual void PreUpdate() = 0;
        virtual void PostUpdate() = 0;

        friend class Renderer;
    };
}