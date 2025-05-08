#pragma once

#include <glm/glm.hpp>

#include <string>

namespace Mosaic
{
    struct ApplicationData;

    enum class RendererVSync
    {
        Disabled,
        Strict,
        Relaxed,
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

        RendererVSync mVSync;

        std::string mConfigPath;

        friend class Renderer;
    };

    class Renderer
    {
    public:
        Renderer(ApplicationData& applicationData);

        glm::fvec4 GetClearColour() const;
        void SetClearColour(const glm::fvec4& colour);

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
        RendererAPI mAPI;
        RendererVSync mVSync;

        std::string mConfigPath;

        friend class Application;
        friend class VulkanRenderer;
        friend class OpenGLRenderer;
    };
}