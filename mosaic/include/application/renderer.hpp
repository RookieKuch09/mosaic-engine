#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Mosaic
{
    struct ApplicationData;

    enum class VSyncMode
    {
        Disabled,
        StrictVSync,
        RelaxedVSync,
    };

    enum class RendererAPI
    {
        Vulkan,
        OpenGL,
    };

    class RendererInterface
    {
    protected:
        virtual void Create() = 0;

        virtual void PreUpdate() = 0;
        virtual void PostUpdate() = 0;

        virtual void LoadConfig() = 0;

        glm::fvec4 mClearColour;
        VSyncMode mVSyncMode;

        std::string mConfigPath;

        friend class Renderer;
    };

    class Renderer
    {
    public:
        Renderer(ApplicationData& applicationData);

        glm::fvec4 GetClearColour() const;
        void SetClearColour(const glm::fvec4& colour);

        VSyncMode GetVSyncMode() const;

        RendererAPI GetRendererAPI() const;

        std::string GetConfigPath() const;
        void SetConfigPath(const std::string& path);

    private:
        void Create();

        void PreUpdate();
        void PostUpdate();

        RendererInterface* mRendererInstance;
        ApplicationData* mApplicationData;

        void LoadConfig();

        glm::fvec4 mClearColour;

        VSyncMode mVSyncMode;
        RendererAPI mAPI;

        std::string mConfigPath;

        friend class Application;
        friend class VulkanRenderer;
        friend class OpenGLRenderer;
    };
}