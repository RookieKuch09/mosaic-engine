#include "../../../include/rendering/vulkan/renderer.hpp"

#include "../../../include/application/application.hpp"
#include "../../../include/application/console.hpp"

Mosaic::VulkanRenderer::VulkanRenderer(ApplicationData* applicationData)
    : mApplicationData(applicationData), mRebuildSwapchainSuboptimal(false), mRebuildSwapchainOutOfDate(false)
{
    mApplicationData->EventManager.Subscribe(this, &VulkanRenderer::ResizeCallback);
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

    mRenderPass.Create(mDevice, mSurface);

    mQueues.Load(mDevice);

    CreateSwapchain();
}

void Mosaic::VulkanRenderer::CreateSwapchain()
{
    mDevice.WaitIdle();

    mCommandSystem.Reset();

    for (auto& framebuffer : mFramebuffers)
    {
        framebuffer.Reset();
    }

    mFramebuffers.clear();

    mSwapchain.Reset();

    mSwapchain.Create(mApplicationData->Window, mDevice, mPhysicalDevice, mSurface, mVSync);
    mSwapchain.CreateSyncObjects(mDevice);

    mFramebuffers.resize(mSwapchain.GetImageCount());

    for (std::uint32_t index = 0; index < mSwapchain.GetImageCount(); index++)
    {
        auto& framebuffer = mFramebuffers[index];

        framebuffer.GetIndex() = index;
        framebuffer.Create(mDevice, mSurface, mRenderPass, mSwapchain);
    }

    mCommandSystem.Create(mDevice, mQueues);
    mCommandSystem.AllocateCommandBuffers(mDevice, mSwapchain);

    if (mRebuildSwapchainOutOfDate or mRebuildSwapchainSuboptimal)
    {
        Console::LogSuccess("Successfully rebuilt swapchain");
    }
}

void Mosaic::VulkanRenderer::Update()
{
    if (mRebuildSwapchainOutOfDate or mRebuildSwapchainSuboptimal)
    {
        CreateSwapchain();

        mRebuildSwapchainSuboptimal = false;
        mRebuildSwapchainOutOfDate = false;
    }

    auto current = mSwapchain.GetCurrentFrame();
    auto& frameSync = mSwapchain.GetSyncFrames()[current];

    mDevice.AwaitFences(mSwapchain);

    std::uint32_t imageIndex = mDevice.GetNextImageIndex(*this, mSwapchain);

    if (mRebuildSwapchainOutOfDate or mRebuildSwapchainSuboptimal)
    {
        return;
    }

    mCommandSystem.BeginFrame(imageIndex);
    mCommandSystem.RecordCommands(mRenderPass, mFramebuffers[imageIndex], mSwapchain, imageIndex, {mClearColour.x, mClearColour.y, mClearColour.z, mClearColour.w});
    mCommandSystem.EndFrame(imageIndex);

    VulkanFrameSubmitDescriptor frameSubmitDescriptor = {
        mCommandSystem.GetCommandBuffer(imageIndex),
        frameSync.ImageAvailable.get(),
        frameSync.RenderFinished.get(),
        frameSync.InFlight.get(),
        vk::PipelineStageFlagBits::eColorAttachmentOutput};

    mCommandSystem.SubmitFrame(mQueues.GetGraphicsQueue(), frameSubmitDescriptor);

    mSwapchain.PresentFrame(*this, mQueues, imageIndex);

    if (mRebuildSwapchainOutOfDate)
    {
        return;
    }

    mSwapchain.IncrementFrame();
}

void Mosaic::VulkanRenderer::LoadConfig()
{
}

void Mosaic::VulkanRenderer::ResizeCallback(const WindowResizeEvent& event)
{
    mWindowSize = event.Size;

    mRebuildSwapchainOutOfDate = true;
}