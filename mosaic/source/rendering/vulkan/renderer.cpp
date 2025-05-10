#include "../../../include/rendering/vulkan/renderer.hpp"

#include "../../../include/application/application.hpp"
#include "../../../include/application/console.hpp"

Mosaic::VulkanRenderer::VulkanRenderer(ApplicationData* applicationData)
    : mApplicationData(applicationData)
{
}

void Mosaic::VulkanRenderer::Create()
{
    mApplicationData->Window.InitialiseVulkan();

    mInstance.SelectWindowExtensions(mApplicationData->Window);
    mInstance.SelectExtensions({}, {});
    mInstance.SelectLayers({"VK_LAYER_KHRONOS_validation"});
    mInstance.Create();

    mPhysicalDevice.Select(mInstance);

    mSurface.Create(mApplicationData->Window, mInstance);
    mSurface.SelectFormat(mPhysicalDevice, vk::Format::eR8G8B8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear);

    mQueues.Discover(mPhysicalDevice, mSurface);

    mDevice.GetExtensions(mPhysicalDevice, {}, {"VK_EXT_buffer_device_address"});
    mDevice.Create(mQueues, mPhysicalDevice);

    mQueues.Load(mDevice);

    mSwapchain.Create(mApplicationData->Window, mDevice, mPhysicalDevice, mSurface, mVSync);
    mSwapchain.CreateSyncObjects(mDevice);

    mRenderPass.Create(mDevice, mSurface);

    mFramebuffers.resize(mSwapchain.GetImageCount());

    for (std::uint32_t index = 0; index < mSwapchain.GetImageCount(); index++)
    {
        auto& framebuffer = mFramebuffers[index];

        framebuffer.GetIndex() = index;
        framebuffer.Create(mDevice, mSurface, mRenderPass, mSwapchain);
    }

    mCommandSystem.Create(mDevice, mQueues);
    mCommandSystem.AllocateCommandBuffers(mDevice, mSwapchain);
}

void Mosaic::VulkanRenderer::Update()
{
}

void Mosaic::VulkanRenderer::LoadConfig()
{
}
