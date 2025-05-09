#include "../../../include/rendering/vulkan/renderer.hpp"

#include "../../../include/application/application.hpp"

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

    mQueues.Discover(mPhysicalDevice);

    mDevice.GetExtensions(mPhysicalDevice, {}, {"VK_EXT_buffer_device_address"});
    mDevice.Create(mQueues, mPhysicalDevice);

    mQueues.Load(mDevice);

    mSurface.Create(mApplicationData->Window, mInstance);
}

void Mosaic::VulkanRenderer::PreUpdate()
{
}

void Mosaic::VulkanRenderer::PostUpdate()
{
}

void Mosaic::VulkanRenderer::LoadConfig()
{
}
