#include "../../include/application/renderer.hpp"
#include "../../include/application/application.hpp"

#include "../../include/rendering/opengl/renderer.hpp"
#include "../../include/rendering/vulkan/renderer.hpp"

#include "../../include/utilities/config.hpp"

#include <array>

Mosaic::Renderer::Renderer(ApplicationData& applicationData)
    : mApplicationData(&applicationData), mConfigPath("")
{
}

glm::fvec4 Mosaic::Renderer::GetClearColour() const
{
    return mClearColour;
}

void Mosaic::Renderer::SetClearColour(const glm::fvec4& colour)
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

void Mosaic::Renderer::Create()
{
    switch (mAPI)
    {
        case (RendererAPI::Vulkan):
        {
            mRendererInstance = new VulkanRenderer(mApplicationData);

            break;
        }
        case (RendererAPI::OpenGL):
        {
            mRendererInstance = new OpenGLRenderer(mApplicationData);

            break;
        }
    }

    mRendererInstance->mClearColour = mClearColour;
    mRendererInstance->mVSync = mVSync;
    mRendererInstance->mConfigPath = mConfigPath;

    mRendererInstance->LoadConfig();
    mRendererInstance->Create();
}

void Mosaic::Renderer::Update()
{
    mRendererInstance->Update();
}

void Mosaic::Renderer::LoadConfig()
{
    TOMLFile config;

    config.Open(mConfigPath);

    auto api = config.Get<std::string>("Renderer.API");
    auto clearColour = config.Get<float, 4>("Renderer.ClearColour", {0.0, 0.0, 0.0, 1.0});
    auto vsync = config.Get<std::string>("Renderer.VSync");

    if (api == "OpenGL")
    {
        mAPI = RendererAPI::OpenGL;
    }
    else if (api == "Vulkan")
    {
        mAPI = RendererAPI::Vulkan;
    }
    else
    {
        Console::LogError("Invalid rendering API \"{}\"", api);
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
        Console::Throw("Unsupported VSync mode for Renderer: {}", vsync);
    }

    mClearColour.r = clearColour[0];
    mClearColour.g = clearColour[1];
    mClearColour.b = clearColour[2];
    mClearColour.a = clearColour[3];
}
