#include "../../../include/rendering/vulkan/swapchain.hpp"
#include "../../../include/rendering/vulkan/devices.hpp"
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

    auto imageViewResult = device.Get().createImageViewUnique(createInfo);

    if (imageViewResult.result != vk::Result::eSuccess)
    {
        Console::Throw("Error creating vk::ImageView for vk::Framebuffer: {}", vk::to_string(imageViewResult.result));
    }

    mImageView = std::move(imageViewResult.value);

    vk::FramebufferCreateInfo framebufferCreateInfo = {
        {},
        renderPass.GetRenderPass(),
        1,
        &mImageView.get(),
        swapchain.GetExtent().width,
        swapchain.GetExtent().height,
        1};

    auto framebufferResult = device.Get().createFramebufferUnique(framebufferCreateInfo);

    if (framebufferResult.result != vk::Result::eSuccess)
    {
        Console::Throw("Error creating vk::Framebuffer: {}", vk::to_string(framebufferResult.result));
    }

    mFramebuffer = std::move(framebufferResult.value);
}

vk::Framebuffer& Mosaic::VulkanFramebuffer::GetFramebuffer()
{
    return *mFramebuffer;
}

vk::ImageView& Mosaic::VulkanFramebuffer::GetImageView()
{
    return *mImageView;
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

    auto result = device.Get().createRenderPassUnique(renderPassInfo);

    if (result.result != vk::Result::eSuccess)
    {
        Console::Throw("Error creating vk::RenderPass: {}", vk::to_string(result.result));
    }

    mRenderPass = std::move(result.value);
}

vk::RenderPass& Mosaic::VulkanRenderPass::GetRenderPass()
{
    return *mRenderPass;
}

void Mosaic::VulkanSwapchain::Create(Window& window, VulkanDevice& device, VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface, RendererVSync vsync)
{
    auto presentResult = physicalDevice.Get().getSurfacePresentModesKHR(surface.GetHandle());
    auto capabilitiesResult = physicalDevice.Get().getSurfaceCapabilitiesKHR(surface.GetHandle());

    if (presentResult.result != vk::Result::eSuccess)
    {
        Console::Throw("Error querying vk::Surface supported presentation modes: {}", vk::to_string(presentResult.result));
    }

    if (capabilitiesResult.result != vk::Result::eSuccess)
    {
        Console::Throw("Error querying vk::Surface capabilities: {}", vk::to_string(presentResult.result));
    }

    auto& presentModes = presentResult.value;
    auto& capabilities = capabilitiesResult.value;

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
            std::clamp(window.GetSize().x, capabilities.minImageExtent.width, capabilities.maxImageExtent.width),
            std::clamp(window.GetSize().y, capabilities.minImageExtent.height, capabilities.maxImageExtent.height)};

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

    auto result = device.Get().createSwapchainKHRUnique(createInfo);

    if (result.result != vk::Result::eSuccess)
    {
        Console::Throw("Error creating vk::Swapchain: {}", vk::to_string(result.result));
    }

    mSwapchain = std::move(result.value);

    auto imagesResult = device.Get().getSwapchainImagesKHR(mSwapchain.get());

    if (imagesResult.result != vk::Result::eSuccess)
    {
        Console::Throw("Error getting vk::Swapchain images: {}", vk::to_string(imagesResult.result));
    }

    mSwapchainImages = std::move(imagesResult.value);
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