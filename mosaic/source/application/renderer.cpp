#include "../../include/application/renderer.hpp"
#include "../../include/application/application.hpp"

#include "../../include/rendering/vulkan/renderer.hpp"

#include "../../include/utilities/config.hpp"

#include <array>

Mosaic::Renderer::Renderer(ApplicationData& applicationData)
    : mApplicationData(&applicationData), mConfigPath("config/renderer.toml")
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

Mosaic::VSyncMode Mosaic::Renderer::GetVSyncMode() const
{
    return mVSyncMode;
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
    LoadConfig();

    switch (mAPI)
    {
        case (RendererAPI::Vulkan):
        {
            mRendererInstance = new VulkanRenderer(mApplicationData);

            break;
        }
        case (RendererAPI::OpenGL):
        {
            throw;

            break;
        }
    }

    mRendererInstance->mClearColour = mClearColour;
    mRendererInstance->mVSyncMode = mVSyncMode;
    mRendererInstance->mConfigPath = mConfigPath;

    mRendererInstance->Create();
}

void Mosaic::Renderer::PreUpdate()
{
    mRendererInstance->PreUpdate();
}

void Mosaic::Renderer::PostUpdate()
{
    mRendererInstance->PostUpdate();
}

void Mosaic::Renderer::LoadConfig()
{
    TOMLFile config;

    config.Open(mConfigPath);

    auto api = config.Get<std::string>("renderer.api");
    std::array clearColour = config.Get<float, 4>("renderer.clearColour", {0.0, 0.0, 0.0, 1.0});
    auto vsyncMode = config.Get<std::string>("renderer.vsyncMode");

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
        mApplicationData->Console.LogError("Invalid rendering API \"{}\"", api);
    }

    if (vsyncMode == "Disabled")
    {
        mVSyncMode = VSyncMode::Disabled;
    }
    else if (vsyncMode == "StrictVSync")
    {
        mVSyncMode = VSyncMode::StrictVSync;
    }
    else if (vsyncMode == "RelaxedVSync")
    {
        mVSyncMode = VSyncMode::RelaxedVSync;
    }
    else
    {
        mApplicationData->Console.LogError("Invalid VSync mode \"{}\"", vsyncMode);
    }

    mClearColour.r = clearColour[0];
    mClearColour.g = clearColour[1];
    mClearColour.b = clearColour[2];
    mClearColour.a = clearColour[3];
}
