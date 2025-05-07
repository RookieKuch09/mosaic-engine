#include "../../../include/rendering/vulkan/swapchain.hpp"
#include "../../../include/application/application.hpp"
#include <limits>

void Mosaic::Backend::Vulkan::CreateSwapchain(
    ApplicationData* applicationData,
    vk::UniqueDevice& device,
    vk::UniqueSurfaceKHR& surface,
    vk::SurfaceFormatKHR& format,
    vk::Extent2D extent,
    vk::PresentModeKHR presentMode,
    vk::UniqueSwapchainKHR& swapchain)
{
    vk::SwapchainCreateInfoKHR createInfo{
        {},
        *surface,
        3,
        format.format,
        format.colorSpace,
        extent,
        1,
        vk::ImageUsageFlagBits::eColorAttachment,
        vk::SharingMode::eExclusive,
        0,
        nullptr,
        vk::SurfaceTransformFlagBitsKHR::eIdentity,
        vk::CompositeAlphaFlagBitsKHR::eOpaque,
        presentMode,
        VK_TRUE,
        nullptr};

    try
    {
        swapchain = device->createSwapchainKHRUnique(createInfo);
    }
    catch (vk::SystemError const& error)
    {
        applicationData->Console.LogError("Failed to create Vulkan swapchain: {}", error.what());
    }

    if (not swapchain)
    {
        applicationData->Console.LogError("Failed to create swapchain (unknown error)");
    }
}

void Mosaic::Backend::Vulkan::GetSwapchainData(
    ApplicationData* applicationData,
    vk::PhysicalDevice& physicalDevice,
    vk::UniqueSurfaceKHR& surface,
    vk::SurfaceFormatKHR& format,
    vk::Extent2D& extent,
    vk::PresentModeKHR preferredMode,
    vk::PresentModeKHR& presentMode,
    uint32_t& imageCount)
{
    auto formats = physicalDevice.getSurfaceFormatsKHR(*surface);
    auto presentModes = physicalDevice.getSurfacePresentModesKHR(*surface);
    auto caps = physicalDevice.getSurfaceCapabilitiesKHR(*surface);

    if (formats.empty() or presentModes.empty())
    {
        applicationData->Console.LogError("Surface does not support any formats or present modes");
    }

    vk::Format favFmt = vk::Format::eB8G8R8A8Unorm;
    vk::ColorSpaceKHR favSpace = vk::ColorSpaceKHR::eSrgbNonlinear;

    auto match = [&](auto const& f)
    {
        return f.format == favFmt and f.colorSpace == favSpace;
    };

    auto itf = std::find_if(formats.begin(), formats.end(), match);

    format = (itf != formats.end() ? *itf : formats[0]);

    if (std::ranges::find(presentModes, preferredMode) != presentModes.end())
    {
        presentMode = preferredMode;
    }
    else if (std::ranges::find(presentModes, vk::PresentModeKHR::eMailbox) != presentModes.end())
    {
        presentMode = vk::PresentModeKHR::eMailbox;
    }
    else if (std::ranges::find(presentModes, vk::PresentModeKHR::eImmediate) != presentModes.end())
    {
        presentMode = vk::PresentModeKHR::eImmediate;
    }
    else
    {
        presentMode = vk::PresentModeKHR::eFifo;
    }

    uint32_t minCount = caps.minImageCount;
    uint32_t maxCount = caps.maxImageCount == 0 ? std::numeric_limits<std::uint32_t>::max() : caps.maxImageCount;

    imageCount = std::clamp(2u, minCount, maxCount);

    if (caps.currentExtent.width != std::numeric_limits<std::uint32_t>::max())
    {
        extent = caps.currentExtent;
    }
    else
    {
        vk::Extent2D chosen{
            std::clamp(caps.minImageExtent.width, applicationData->Window.GetSize().x, caps.maxImageExtent.width),
            std::clamp(caps.minImageExtent.height, applicationData->Window.GetSize().y, caps.maxImageExtent.height)};

        extent = chosen;
    }
}

void Mosaic::Backend::Vulkan::CreateRenderPass(
    ApplicationData* applicationData,
    vk::UniqueDevice& device,
    vk::SurfaceFormatKHR& format,
    vk::UniqueRenderPass& renderPass)
{
    vk::AttachmentDescription colourAttachment = {
        {},
        format.format,
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

    try
    {
        renderPass = device->createRenderPassUnique(renderPassInfo);
    }
    catch (const vk::SystemError& error)
    {
        applicationData->Console.LogError("Failed to create render pass: {}", error.what());
    }
}

void Mosaic::Backend::Vulkan::CreateSwapchainImages(
    ApplicationData* applicationData,
    vk::UniqueDevice& device,
    vk::SurfaceFormatKHR& format,
    std::vector<vk::Image>& swapchainImages,
    std::vector<vk::UniqueImageView>& imageViews)
{
    for (auto& image : swapchainImages)
    {
        vk::ImageViewCreateInfo createInfo = {
            {},
            image,
            vk::ImageViewType::e2D,
            format.format,
            vk::ComponentMapping(),
            vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)};

        try
        {
            imageViews.push_back(device->createImageViewUnique(createInfo));
        }
        catch (const vk::SystemError& error)
        {
            applicationData->Console.LogError("Failed to create framebuffer image: {}", error.what());
        }
    }
}

void Mosaic::Backend::Vulkan::CreateFramebuffers(
    ApplicationData* applicationData,
    vk::UniqueDevice& device,
    vk::SurfaceFormatKHR& format,
    vk::UniqueRenderPass& renderPass,
    vk::Extent2D& swapchainExtent,
    std::vector<vk::UniqueFramebuffer>& swapchainFramebuffers,
    std::vector<vk::UniqueImageView>& swapchainImageViews)
{
    for (auto& image : swapchainImageViews)
    {
        vk::FramebufferCreateInfo framebufferCreateInfo = {
            {},
            *renderPass,
            1,
            &*image,
            swapchainExtent.width,
            swapchainExtent.height,
            1};

        try
        {
            swapchainFramebuffers.push_back(device->createFramebufferUnique(framebufferCreateInfo));
        }
        catch (const vk::SystemError& error)
        {
            applicationData->Console.LogError("Failed to create framebuffer: {}", error.what());
        }
    }
}