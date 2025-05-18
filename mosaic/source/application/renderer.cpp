#include "application/application.hpp"

#include "rendering/opengl/renderer.hpp"
#include "rendering/vulkan/renderer.hpp"

#include "utilities/config.hpp"

Mosaic::Renderer::Renderer(ApplicationData& applicationData)
    : mApplicationData(applicationData), mConfigPath(""), mBackend(nullptr)
{
}

Mosaic::Vector4<float> Mosaic::Renderer::GetClearColour() const
{
    return mClearColour;
}

void Mosaic::Renderer::SetClearColour(const Vector4<float>& colour)
{
    mClearColour = colour;
}

Mosaic::RendererAPI Mosaic::Renderer::GetRendererAPI() const
{
    return mAPI;
}

std::string Mosaic::Renderer::GetConfigPath() const
{
    return mConfigPath;
}

void Mosaic::Renderer::SetConfigPath(const std::string& path)
{
    mConfigPath = path;
}

void Mosaic::Renderer::LoadConfig()
{
    TOMLFile config;

    config.Open(mConfigPath);

    auto api = config.Get<std::string>("Renderer.API");
    auto clearColour = config.Get<float32, 4>("Renderer.ClearColour", {0.0, 0.0, 0.0, 1.0});
    auto vsync = config.Get<std::string>("Renderer.VSync");

    if (api == "OpenGL")
    {
        mAPI = RendererAPI::OpenGL;
        mBackend = new OpenGLRenderer(*this);
    }
    else if (api == "Vulkan")
    {
        mAPI = RendererAPI::Vulkan;
        mBackend = new VulkanRenderer(*this);
    }
    else
    {
        Console::Throw("Unsupported rendering API \"{}\"", api);
    }

    if (vsync == "Disabled")
    {
        mVSync = RendererVSync::Disabled;
    }
    else if (vsync == "Strict")
    {
        mVSync = RendererVSync::Strict;
    }
    else if (vsync == "Relaxed")
    {
        mVSync = RendererVSync::Relaxed;
    }
    else
    {
        Console::Throw("Unsupported VSync mode \"{}\"", vsync);
    }

    mClearColour.X = clearColour[0];
    mClearColour.Y = clearColour[1];
    mClearColour.Z = clearColour[2];
    mClearColour.W = clearColour[3];

    mBackend->LoadConfig();
}

void Mosaic::Renderer::Create()
{
    mBackend->Create();
}

void Mosaic::Renderer::Update()
{
    mBackend->Update();
}