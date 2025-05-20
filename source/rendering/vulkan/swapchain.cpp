#include "rendering/vulkan/swapchain.hpp"
#include "rendering/vulkan/devices.hpp"
#include "rendering/vulkan/queues.hpp"
#include "rendering/vulkan/renderer.hpp"
#include "rendering/vulkan/surface.hpp"

#include "application/console.hpp"
#include "application/window.hpp"
#include "rendering/renderer.hpp"

namespace Mosaic::Internal::Rendering
{
    void VulkanFramebuffer::Create(VulkanDevice& device, VulkanSurface& surface, VulkanRenderPass& renderPass, VulkanSwapchain& swapchain)
    {
        auto& image = swapchain.GetImage(mIndex);

        vk::ImageViewCreateInfo createInfo = {
            {},
            image,
            vk::ImageViewType::e2D,
            surface.GetFormat().format,
            vk::ComponentMapping(),
            vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)};

        mImageView = device.Get().createImageViewUnique(createInfo);

        vk::FramebufferCreateInfo framebufferCreateInfo = {
            {},
            renderPass.GetRenderPass(),
            1,
            &mImageView.get(),
            swapchain.GetExtent().width,
            swapchain.GetExtent().height,
            1};

        mFramebuffer = device.Get().createFramebufferUnique(framebufferCreateInfo);
    }

    void VulkanFramebuffer::Reset()
    {
        if (mFramebuffer)
        {
            mFramebuffer.reset();
            mImageView.reset();
        }
    }

    vk::Framebuffer& VulkanFramebuffer::GetFramebuffer()
    {
        return mFramebuffer.get();
    }

    vk::ImageView& VulkanFramebuffer::GetImageView()
    {
        return mImageView.get();
    }

    Types::UI32& VulkanFramebuffer::GetIndex()
    {
        return mIndex;
    }

    void VulkanRenderPass::Create(VulkanDevice& device, VulkanSurface& surface)
    {
        vk::AttachmentDescription colourAttachment = {
            {},
            surface.GetFormat().format,
            vk::SampleCountFlagBits::e1,
            vk::AttachmentLoadOp::eClear,
            vk::AttachmentStoreOp::eStore,
            vk::AttachmentLoadOp::eDontCare,
            vk::AttachmentStoreOp::eDontCare,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::ePresentSrcKHR};

        vk::AttachmentReference colourAttachmentRef = {
            0,
            vk::ImageLayout::eColorAttachmentOptimal};

        vk::SubpassDescription subpass = {
            {},
            vk::PipelineBindPoint::eGraphics,
            0,
            nullptr,
            1,
            &colourAttachmentRef};

        vk::SubpassDependency dependency = {
            VK_SUBPASS_EXTERNAL,
            0,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            vk::PipelineStageFlagBits::eColorAttachmentOutput,
            {},
            vk::AccessFlagBits::eColorAttachmentWrite};

        vk::RenderPassCreateInfo renderPassInfo = {
            {},
            1,
            &colourAttachment,
            1,
            &subpass,
            1,
            &dependency};

        mRenderPass = device.Get().createRenderPassUnique(renderPassInfo);
    }

    vk::RenderPass& VulkanRenderPass::GetRenderPass()
    {
        return mRenderPass.get();
    }

    void VulkanSwapchain::Create(const Types::Vector2<Types::UI32>& size, VulkanDevice& device, VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface, RendererVSync vsync)
    {
        auto presentModes = physicalDevice.Get().getSurfacePresentModesKHR(surface.GetHandle());
        auto capabilities = physicalDevice.Get().getSurfaceCapabilitiesKHR(surface.GetHandle());

        if (presentModes.empty())
        {
            Console::Throw("vk::Surface does not support any present modes");
        }

        auto supports = [&](vk::PresentModeKHR mode)
        {
            return std::find(presentModes.begin(), presentModes.end(), mode) != presentModes.end();
        };

        std::vector<vk::PresentModeKHR> fallbackList;

        switch (vsync)
        {
            case (RendererVSync::Disabled):
            {
                fallbackList = {
                    vk::PresentModeKHR::eImmediate,
                    vk::PresentModeKHR::eMailbox,
                    vk::PresentModeKHR::eFifoLatestReadyEXT,
                    vk::PresentModeKHR::eFifoRelaxed,
                    vk::PresentModeKHR::eFifo};
                break;
            }

            case (RendererVSync::Strict):
            {
                fallbackList = {
                    vk::PresentModeKHR::eMailbox,
                    vk::PresentModeKHR::eFifoLatestReadyEXT,
                    vk::PresentModeKHR::eFifoRelaxed,
                    vk::PresentModeKHR::eFifo};
                break;
            }

            case (RendererVSync::Relaxed):
            {
                fallbackList = {
                    vk::PresentModeKHR::eFifoLatestReadyEXT,
                    vk::PresentModeKHR::eFifoRelaxed,
                    vk::PresentModeKHR::eFifo};
                break;
            }
        }

        auto it = std::find_if(fallbackList.begin(), fallbackList.end(), supports);
        mPresentMode = (it != fallbackList.end() ? *it : vk::PresentModeKHR::eFifo);

        mImageCount = std::max(2u, capabilities.minImageCount);
        mFramesInFlight = std::clamp(mImageCount - 1, 1u, 3u);

        if (capabilities.maxImageCount > 0)
        {
            mImageCount = std::min(mImageCount + 1, capabilities.maxImageCount);
        }
        else
        {
            mImageCount += 1;
        }

        if (capabilities.currentExtent.width != std::numeric_limits<Types::UI32>::max())
        {
            mSwapchainExtent = capabilities.currentExtent;
        }
        else
        {
            vk::Extent2D chosen{
                std::clamp(size.X, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
                std::clamp(size.Y, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)};

            mSwapchainExtent = chosen;
        }

        vk::SwapchainCreateInfoKHR createInfo{
            {},
            surface.GetHandle(),
            mImageCount,
            surface.GetFormat().format,
            surface.GetFormat().colorSpace,
            mSwapchainExtent,
            1,
            vk::ImageUsageFlagBits::eColorAttachment,
            vk::SharingMode::eExclusive,
            0,
            nullptr,
            vk::SurfaceTransformFlagBitsKHR::eIdentity,
            vk::CompositeAlphaFlagBitsKHR::eOpaque,
            mPresentMode,
            VK_TRUE,
            mSwapchain ? mSwapchain.get() : nullptr};

        mSwapchain = device.Get().createSwapchainKHRUnique(createInfo);

        mSwapchainImages = device.Get().getSwapchainImagesKHR(mSwapchain.get());
    }

    void VulkanSwapchain::CreateSyncObjects(VulkanDevice& device)
    {
        mSyncFrames.clear();
        mSyncFrames.reserve(mFramesInFlight);
        mImagesInFlight.resize(mImageCount, VK_NULL_HANDLE);

        vk::SemaphoreCreateInfo semaphoreInfo{};
        vk::FenceCreateInfo fenceInfo{vk::FenceCreateFlagBits::eSignaled};

        for (Types::UI32 index = 0; index < mFramesInFlight; index++)
        {
            VulkanFrameSyncObjects frameSync{};

            frameSync.ImageAvailable = device.Get().createSemaphoreUnique(semaphoreInfo);
            frameSync.RenderFinished = device.Get().createSemaphoreUnique(semaphoreInfo);
            frameSync.InFlight = device.Get().createFenceUnique(fenceInfo);

            mSyncFrames.emplace_back(std::move(frameSync));
        }
    }

    vk::SwapchainKHR& VulkanSwapchain::GetSwapchain()
    {
        return *mSwapchain;
    }

    vk::Extent2D& VulkanSwapchain::GetExtent()
    {
        return mSwapchainExtent;
    }

    vk::Image& VulkanSwapchain::GetImage(Types::UI32 index)
    {
        return mSwapchainImages[index];
    }

    Types::UI32 VulkanSwapchain::GetImageCount()
    {
        return mImageCount;
    }

    Types::UI32 VulkanSwapchain::GetInFlightFrames()
    {
        return mFramesInFlight;
    }

    Types::UI32 VulkanSwapchain::GetCurrentFrame()
    {
        return mCurrentFrame;
    }

    void VulkanSwapchain::IncrementFrame()
    {
        mCurrentFrame = (mCurrentFrame + 1) % mFramesInFlight;
    }

    std::vector<VulkanFrameSyncObjects>& VulkanSwapchain::GetSyncFrames()
    {
        return mSyncFrames;
    }

    void VulkanSwapchain::PresentFrame(VulkanRenderer& renderer, VulkanQueues& queues, Types::UI32 imageIndex)
    {
        vk::PresentInfoKHR presentInfo = {
            1, &mSyncFrames[GetCurrentFrame()].RenderFinished.get(),
            1, &mSwapchain.get(),
            &imageIndex};

        try
        {
            auto result = queues.GetPresentQueue().presentKHR(presentInfo);

            if (result == vk::Result::eSuboptimalKHR)
            {
                renderer.mRebuildSwapchainSuboptimal = true;
            }
            else if (result == vk::Result::eErrorOutOfDateKHR)
            {
                renderer.mRebuildSwapchainOutOfDate = true;
            }
            else if (result != vk::Result::eSuccess)
            {
                Console::Throw("Failed to present frame: {}", vk::to_string(result));
            }
        }
        catch (const vk::SystemError& error)
        {
            auto result = static_cast<vk::Result>(error.code().value());

            if (result == vk::Result::eSuboptimalKHR)
            {
                renderer.mRebuildSwapchainSuboptimal = true;
            }
            else if (result == vk::Result::eErrorOutOfDateKHR)
            {
                renderer.mRebuildSwapchainOutOfDate = true;
            }
            else if (result != vk::Result::eSuccess)
            {
                Console::Throw("Failed to present frame: {}", vk::to_string(result));
            }
        }
    }

    void VulkanSwapchain::Reset()
    {
        if (mSwapchain)
        {
            mSwapchain.reset();

            mSwapchainImages.clear();
            mSyncFrames.clear();
            mImagesInFlight.clear();

            mCurrentFrame = 0;
        }
    }
}