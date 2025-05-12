#include "../../../include/rendering/vulkan/swapchain.hpp"
#include "../../../include/rendering/vulkan/devices.hpp"
#include "../../../include/rendering/vulkan/queues.hpp"
#include "../../../include/rendering/vulkan/renderer.hpp"
#include "../../../include/rendering/vulkan/surface.hpp"

#include "../../../include/application/console.hpp"
#include "../../../include/application/renderer.hpp"
#include "../../../include/application/window.hpp"

void Mosaic::VulkanFramebuffer::Create(VulkanDevice& device, VulkanSurface& surface, VulkanRenderPass& renderPass, VulkanSwapchain& swapchain)
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

void Mosaic::VulkanFramebuffer::Reset()
{
    if (mFramebuffer)
    {
        mFramebuffer.reset();
        mImageView.reset();
    }
}

vk::Framebuffer& Mosaic::VulkanFramebuffer::GetFramebuffer()
{
    return mFramebuffer.get();
}

vk::ImageView& Mosaic::VulkanFramebuffer::GetImageView()
{
    return mImageView.get();
}

std::uint32_t& Mosaic::VulkanFramebuffer::GetIndex()
{
    return mIndex;
}

void Mosaic::VulkanRenderPass::Create(VulkanDevice& device, VulkanSurface& surface)
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

vk::RenderPass& Mosaic::VulkanRenderPass::GetRenderPass()
{
    return mRenderPass.get();
}

void Mosaic::VulkanSwapchain::Create(Window& window, VulkanDevice& device, VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface, RendererVSync vsync)
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

    if (capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max())
    {
        mSwapchainExtent = capabilities.currentExtent;
    }
    else
    {
        vk::Extent2D chosen{
            std::clamp(window.GetSize().X, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
            std::clamp(window.GetSize().Y, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)};

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

void Mosaic::VulkanSwapchain::CreateSyncObjects(VulkanDevice& device)
{
    mSyncFrames.clear();
    mSyncFrames.reserve(mFramesInFlight);
    mImagesInFlight.resize(mImageCount, VK_NULL_HANDLE);

    vk::SemaphoreCreateInfo semaphoreInfo{};
    vk::FenceCreateInfo fenceInfo{vk::FenceCreateFlagBits::eSignaled};

    for (std::uint32_t index = 0; index < mFramesInFlight; index++)
    {
        VulkanFrameSyncObjects frameSync{};

        frameSync.ImageAvailable = device.Get().createSemaphoreUnique(semaphoreInfo);
        frameSync.RenderFinished = device.Get().createSemaphoreUnique(semaphoreInfo);
        frameSync.InFlight = device.Get().createFenceUnique(fenceInfo);

        mSyncFrames.emplace_back(std::move(frameSync));
    }
}

vk::SwapchainKHR& Mosaic::VulkanSwapchain::GetSwapchain()
{
    return *mSwapchain;
}

vk::Extent2D& Mosaic::VulkanSwapchain::GetExtent()
{
    return mSwapchainExtent;
}

vk::Image& Mosaic::VulkanSwapchain::GetImage(std::uint32_t index)
{
    return mSwapchainImages[index];
}

std::uint32_t Mosaic::VulkanSwapchain::GetImageCount()
{
    return mImageCount;
}

std::uint32_t Mosaic::VulkanSwapchain::GetInFlightFrames()
{
    return mFramesInFlight;
}

std::uint32_t Mosaic::VulkanSwapchain::GetCurrentFrame()
{
    return mCurrentFrame;
}

void Mosaic::VulkanSwapchain::IncrementFrame()
{
    mCurrentFrame = (mCurrentFrame + 1) % mFramesInFlight;
}

std::vector<Mosaic::VulkanFrameSyncObjects>& Mosaic::VulkanSwapchain::GetSyncFrames()
{
    return mSyncFrames;
}

void Mosaic::VulkanSwapchain::PresentFrame(VulkanRenderer& renderer, VulkanQueues& queues, std::uint32_t imageIndex)
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

void Mosaic::VulkanSwapchain::Reset()
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