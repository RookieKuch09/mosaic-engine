#pragma once

#include "../utilities/arithmetic.hpp"
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
        virtual void LoadConfig() = 0;
        virtual void Create() = 0;
        virtual void Update() = 0;

        friend class Renderer;
    };

    class Renderer
    {
    public:
        Renderer(ApplicationData& applicationData);

        Vector4<float32> GetClearColour() const;
        RendererAPI GetRendererAPI() const;
        std::string GetConfigPath() const;

        void SetClearColour(const Vector4<float32>& colour);
        void SetConfigPath(const std::string& path);

    protected:
        void LoadConfig();
        void Create();
        void Update();

        ApplicationData& mApplicationData;
        Vector4<float32> mClearColour;
        RendererAPI mAPI;
        RendererVSync mVSync;
        RendererInterface* mBackend;

        std::string mConfigPath;

        friend class Application;
        friend class OpenGLRenderer;
        friend class VulkanRenderer;
    };
}