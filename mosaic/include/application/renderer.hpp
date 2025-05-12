#pragma once

#include "../utilities/vector.hpp"

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

        virtual void Update() = 0;

        virtual void LoadConfig() = 0;

        Vector4<float> mClearColour;

        RendererVSync mVSync;

        std::string mConfigPath;

        friend class Renderer;
    };

    class Renderer
    {
    public:
        Renderer(ApplicationData& applicationData);

        Vector4<float> GetClearColour() const;
        void SetClearColour(const Vector4<float>& colour);

        RendererAPI GetRendererAPI() const;

        std::string GetConfigPath() const;
        void SetConfigPath(const std::string& path);

    private:
        void Create();

        void Update();

        RendererInterface* mRendererInstance;
        ApplicationData* mApplicationData;

        void LoadConfig();

        Vector4<float> mClearColour;
        RendererAPI mAPI;
        RendererVSync mVSync;

        std::string mConfigPath;

        friend class Application;
        friend class VulkanRenderer;
        friend class OpenGLRenderer;
    };
}