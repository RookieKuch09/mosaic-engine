#include "../../include/application/renderer.hpp"
#include "../../include/application/application.hpp"

#include "../../include/rendering/vulkan/renderer.hpp"

Mosaic::Renderer::Renderer(ApplicationData& applicationData)
    : mApplicationData(&applicationData)
{
}

glm::fvec4 Mosaic::Renderer::GetClearColour() const
{
    return mRendererInstance->mClearColour;
}

void Mosaic::Renderer::SetClearColour(const glm::fvec4& colour)
{
    mRendererInstance->mClearColour = colour;
}

Mosaic::VSyncMode Mosaic::Renderer::GetVSyncMode() const
{
    return mRendererInstance->mVSyncMode;
}

void Mosaic::Renderer::SetVSyncMode(VSyncMode mode)
{
    mRendererInstance->mVSyncMode = mode;
}

void Mosaic::Renderer::Create()
{
    switch (mApplicationData->RendererAPI)
    {
        case (ApplicationRendererAPI::Vulkan):
        {
            mRendererInstance = new VulkanRenderer(mApplicationData);

            break;
        }
        case (ApplicationRendererAPI::OpenGL):
        {
            throw;

            break;
        }
    }

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
