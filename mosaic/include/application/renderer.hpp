#pragma once

#include <glm/glm.hpp>

namespace Mosaic
{
    struct ApplicationData;

    enum class VSyncMode
    {
        Disabled,
        StrictVSync,
        RelaxedVSync,
    };

    class RendererInterface
    {
    private:
        virtual void Create() = 0;

        virtual void PreUpdate() = 0;
        virtual void PostUpdate() = 0;

        glm::fvec4 mClearColour;
        VSyncMode mVSyncMode;

        friend class Renderer;
        friend class VulkanRenderer;
        friend class OpenGLRenderer;
    };

    class Renderer
    {
    public:
        Renderer(ApplicationData& applicationData);

        glm::fvec4 GetClearColour() const;
        void SetClearColour(const glm::fvec4& colour);

        VSyncMode GetVSyncMode() const;
        void SetVSyncMode(VSyncMode mode);

    private:
        void Create();

        void PreUpdate();
        void PostUpdate();

        RendererInterface* mRendererInstance;
        ApplicationData* mApplicationData;

        friend class Application;
    };
}