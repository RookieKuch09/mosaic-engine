#include "rendering/vulkan/renderer.hpp"

#include "application/window.hpp"

namespace Mosaic::Internal::Rendering
{
    VulkanRenderer::VulkanRenderer(Renderer& renderer)
        : mRenderer(renderer), mRebuildSwapchainSuboptimal(false), mRebuildSwapchainOutOfDate(false)
    {
        mRenderer.mEventManager.Subscribe(this, &VulkanRenderer::ResizeCallback);
    }

    void VulkanRenderer::Create()
    {
        mRenderer.mWindow.InitialiseVulkan();

        mInstance.SelectWindowExtensions(mRenderer.mWindow);
        mInstance.SelectExtensions({}, {});
        mInstance.SelectLayers({});
        mInstance.Create();

        mPhysicalDevice.Select(mInstance);

        mSurface.Create(mRenderer.mWindow, mInstance);
        mSurface.SelectFormat(mPhysicalDevice, vk::Format::eR8G8B8A8Srgb, vk::ColorSpaceKHR::eSrgbNonlinear);

        mQueues.Discover(mPhysicalDevice, mSurface);

        mDevice.GetExtensions(mPhysicalDevice, {}, {});
        mDevice.Create(mQueues, mPhysicalDevice);

        mRenderPass.Create(mDevice, mSurface);

        mQueues.Load(mDevice);

        CreateSwapchain();
    }

    void VulkanRenderer::CreateSwapchain()
    {
        mDevice.WaitIdle();

        mCommandSystem.Reset();

        for (auto& framebuffer : mFramebuffers)
        {
            framebuffer.Reset();
        }

        mFramebuffers.clear();

        mSwapchain.Reset();

        mSwapchain.Create(mWindowSize, mDevice, mPhysicalDevice, mSurface, mRenderer.mVSync);
        mSwapchain.CreateSyncObjects(mDevice);

        mFramebuffers.resize(mSwapchain.GetImageCount());

        for (Types::UInt32 index = 0; index < mSwapchain.GetImageCount(); index++)
        {
            auto& framebuffer = mFramebuffers[index];

            framebuffer.GetIndex() = index;
            framebuffer.Create(mDevice, mSurface, mRenderPass, mSwapchain);
        }

        mCommandSystem.Create(mDevice, mQueues);
        mCommandSystem.AllocateCommandBuffers(mDevice, mSwapchain);
    }

    void VulkanRenderer::Update()
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

        Types::UInt32 imageIndex = mDevice.GetNextImageIndex(*this, mSwapchain);

        if (mRebuildSwapchainOutOfDate or mRebuildSwapchainSuboptimal)
        {
            return;
        }

        mCommandSystem.BeginFrame(imageIndex);
        mCommandSystem.RecordCommands(mRenderPass, mFramebuffers[imageIndex], mSwapchain, imageIndex, {mRenderer.mClearColour.X, mRenderer.mClearColour.Y, mRenderer.mClearColour.Z, mRenderer.mClearColour.W});
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

    void VulkanRenderer::LoadConfig()
    {
    }

    void VulkanRenderer::ResizeCallback(const Windowing::WindowResizeEvent& event)
    {
        mWindowSize = event.Size;

        mRebuildSwapchainOutOfDate = true;
    }
}