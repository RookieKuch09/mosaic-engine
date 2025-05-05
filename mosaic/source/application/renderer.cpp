#include "../../include/application/application.hpp"

#include "../../include/rendering/vulkan/devices.hpp"
#include "../../include/rendering/vulkan/instance.hpp"

#include <SDL3/SDL_vulkan.h>

Mosaic::Renderer::Renderer(ApplicationData& applicationData)
    : mApplicationData(&applicationData)
{
}

void Mosaic::Renderer::Create()
{
    SDL_Vulkan_LoadLibrary(nullptr);

    CreateInstance();
    CreateDevices();
    CreateSurface();
    CreateSwapchain();
    CreateRenderPass();
    CreateFramebuffers();
    CreateCommandBuffers();
    CreateSyncObjects();

    mCurrentFrame = 0;
}

void Mosaic::Renderer::Update()
{
    mDevice->waitForFences(1, &*mInFlightFences[mCurrentFrame], VK_TRUE, UINT64_MAX);
    mDevice->resetFences(1, &*mInFlightFences[mCurrentFrame]);

    uint32_t imageIndex;
    vk::Result result = mDevice->acquireNextImageKHR(
        *mSwapchain,
        UINT64_MAX,
        *mImageAvailableSemaphores[mCurrentFrame],
        nullptr,
        &imageIndex);

    if (result == vk::Result::eErrorOutOfDateKHR or result == vk::Result::eSuboptimalKHR)
    {
        return;
    }
    else if (result != vk::Result::eSuccess)
    {
        mApplicationData->Console.LogError("Failed to acquire swapchain image: {}", vk::to_string(result));
    }

    RecordCommandBuffer(*mCommandBuffers[imageIndex], imageIndex);

    vk::Semaphore waitSemaphores[] = {*mImageAvailableSemaphores[mCurrentFrame]};
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    vk::Semaphore signalSemaphores[] = {*mRenderFinishedSemaphores[mCurrentFrame]};

    vk::SubmitInfo submitInfo(
        1,
        waitSemaphores,
        waitStages,
        1,
        &*mCommandBuffers[imageIndex],
        1,
        signalSemaphores);

    try
    {
        mGraphicsQueue.submit(submitInfo, *mInFlightFences[mCurrentFrame]);
    }
    catch (const vk::SystemError& error)
    {
        mApplicationData->Console.LogError("Failed to submit command buffer: {}", error.what());
    }

    vk::PresentInfoKHR presentInfo(
        1,
        signalSemaphores,
        1,
        &*mSwapchain,
        &imageIndex,
        nullptr);

    try
    {
        auto presRes = mGraphicsQueue.presentKHR(presentInfo);

        if (presRes == vk::Result::eErrorOutOfDateKHR or presRes == vk::Result::eSuboptimalKHR)
        {
        }
    }
    catch (const vk::SystemError& error)
    {
        mApplicationData->Console.LogError("Failed to present swapchain image: {}", error.what());
    }

    mCurrentFrame = (mCurrentFrame + 1) % static_cast<uint32_t>(mInFlightFences.size());
}

void Mosaic::Renderer::CreateInstance()
{
    unsigned int count = 0;

    const char* const* rawRequestedExtensions = SDL_Vulkan_GetInstanceExtensions(&count);

    std::vector<std::string> requestedExtensions(rawRequestedExtensions, rawRequestedExtensions + count);

    std::vector<std::string> layers;
    std::vector<std::string> extensions;

    Backend::Vulkan::GetLayers(mApplicationData, layers, {});
    Backend::Vulkan::GetInstanceExtensions(mApplicationData, extensions, {});

    Backend::Vulkan::CreateInstance(mApplicationData, mInstance, layers, extensions);
}

void Mosaic::Renderer::CreateDevices()
{
    Backend::Vulkan::SelectPhysicalDevice(mApplicationData, mInstance, mPhysicalDevice);

    std::vector<std::string> extensions;

    Backend::Vulkan::GetPhysicalDeviceExtensions(mPhysicalDevice, extensions);
    Backend::Vulkan::CreateDevice(mApplicationData, mGraphicsQueue, mDevice, mPhysicalDevice, extensions);
}

void Mosaic::Renderer::CreateSurface()
{
    VkSurfaceKHR rawSurface;

    if (!SDL_Vulkan_CreateSurface(mApplicationData->Window.mHandle, *mInstance, nullptr, &rawSurface))
    {
        mApplicationData->Console.LogError("Failed to create Vulkan surface");
    }

    mSurface = vk::UniqueSurfaceKHR(rawSurface, *mInstance);
}

void Mosaic::Renderer::CreateSwapchain()
{
    SelectSurfaceFormatAndPresentMode();

    auto capabilities = mPhysicalDevice.getSurfaceCapabilitiesKHR(*mSurface);

    uint32_t imageCount = capabilities.minImageCount + 1;

    if (capabilities.maxImageCount > 0 and imageCount > capabilities.maxImageCount)
    {
        imageCount = capabilities.maxImageCount;
    }

    vk::SwapchainCreateInfoKHR swapchainCreateInfo(
        {},
        *mSurface,
        imageCount,
        mSurfaceFormat.format,
        mSurfaceFormat.colorSpace,
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
        nullptr);

    try
    {
        mSwapchain = mDevice->createSwapchainKHRUnique(swapchainCreateInfo);
    }
    catch (const vk::SystemError& error)
    {
        mApplicationData->Console.LogError("Failed to create Vulkan swapchain: {}", error.what());
    }

    if (not mSwapchain)
    {
        mApplicationData->Console.LogError("Failed to create swapchain (unknown error)");
    }

    mSwapchainImages = mDevice->getSwapchainImagesKHR(*mSwapchain);

    mSwapchainImageViews.clear();

    for (const auto& image : mSwapchainImages)
    {
        vk::ImageViewCreateInfo viewInfo = {
            {},
            image,
            vk::ImageViewType::e2D,
            mSurfaceFormat.format,
            {vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity,
             vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity},
            {vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1}};

        try
        {
            mSwapchainImageViews.push_back(mDevice->createImageViewUnique(viewInfo));
        }
        catch (const vk::SystemError& error)
        {
            mApplicationData->Console.LogError("Failed to create image view for swapchain image: {}", error.what());
        }
    }

    mImageCount = mSwapchainImages.size();
}

void Mosaic::Renderer::CreateRenderPass()
{
    vk::AttachmentDescription colourAttachment = {
        {},
        mSurfaceFormat.format,
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
        mRenderPass = mDevice->createRenderPassUnique(renderPassInfo);
    }
    catch (const vk::SystemError& error)
    {
        mApplicationData->Console.LogError("Failed to create render pass: {}", error.what());
    }
}

void Mosaic::Renderer::CreateFramebuffers()
{
    mSwapchainImageViews.reserve(mSwapchainImages.size());

    for (auto& image : mSwapchainImages)
    {
        vk::ImageViewCreateInfo createInfo = {
            {},
            image,
            vk::ImageViewType::e2D,
            mSurfaceFormat.format,
            vk::ComponentMapping(),
            vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1)};

        try
        {
            mSwapchainImageViews.push_back(mDevice->createImageViewUnique(createInfo));
        }
        catch (const vk::SystemError& error)
        {
            mApplicationData->Console.LogError("Failed to create framebuffer image: {}", error.what());
        }
    }

    mSwapchainFramebuffers.reserve(mSwapchainImages.size());

    for (auto& image : mSwapchainImageViews)
    {
        vk::FramebufferCreateInfo framebufferCreateInfo = {
            {},
            *mRenderPass,
            1,
            &*image,
            mSwapchainExtent.width,
            mSwapchainExtent.height,
            1};

        try
        {
            mSwapchainFramebuffers.push_back(mDevice->createFramebufferUnique(framebufferCreateInfo));
        }
        catch (const vk::SystemError& error)
        {
            mApplicationData->Console.LogError("Failed to create framebuffer: {}", error.what());
        }
    }
}

void Mosaic::Renderer::CreateCommandBuffers()
{
    vk::CommandPoolCreateInfo poolCreateInfo = {};
    poolCreateInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

    try
    {
        mCommandPool = mDevice->createCommandPoolUnique(poolCreateInfo);

        mCommandBuffers.resize(mSwapchainImages.size());

        vk::CommandBufferAllocateInfo allocateInfo = {};
        allocateInfo.commandPool = *mCommandPool;
        allocateInfo.level = vk::CommandBufferLevel::ePrimary;
        allocateInfo.commandBufferCount = static_cast<uint32_t>(mSwapchainImages.size());

        mCommandBuffers = std::move(mDevice->allocateCommandBuffersUnique(allocateInfo));
    }
    catch (const vk::SystemError& error)
    {
        mApplicationData->Console.LogError("Failed to create command buffers: {}", error.what());
    }
}

void Mosaic::Renderer::CreateSyncObjects()
{
    vk::SemaphoreCreateInfo semaphoreInfo = {};
    vk::FenceCreateInfo fenceInfo = {vk::FenceCreateFlagBits::eSignaled};

    size_t frames = mSwapchainImages.size();

    mImageAvailableSemaphores.resize(frames);
    mRenderFinishedSemaphores.resize(frames);
    mInFlightFences.resize(frames);

    for (size_t i = 0; i < frames; ++i)
    {
        mImageAvailableSemaphores[i] = mDevice->createSemaphoreUnique(semaphoreInfo);
        mRenderFinishedSemaphores[i] = mDevice->createSemaphoreUnique(semaphoreInfo);
        mInFlightFences[i] = mDevice->createFenceUnique(fenceInfo);
    }
}

void Mosaic::Renderer::RecordCommandBuffer(vk::CommandBuffer& commandBuffer, std::uint32_t imageIndex)
{
    if (!mRenderPass)
    {
        mApplicationData->Console.LogError("Render pass is null!");
    }

    if (imageIndex >= mSwapchainFramebuffers.size())
    {
        mApplicationData->Console.LogError("Invalid image index: {} (framebuffers size: {})", imageIndex, mSwapchainFramebuffers.size());
    }

    if (!mSwapchainFramebuffers[imageIndex])
    {
        mApplicationData->Console.LogError("Framebuffer for image {} is null!", imageIndex);
    }

    if (mSwapchainExtent.width == 0 or mSwapchainExtent.height == 0)
    {
        mApplicationData->Console.LogError("Swapchain extent is invalid: {}x{}", mSwapchainExtent.width, mSwapchainExtent.height);
    }

    try
    {
        vk::CommandBufferBeginInfo beginInfo = {};
        commandBuffer.begin(beginInfo);

        vk::RenderPassBeginInfo renderPassBeginInfo = {
            *mRenderPass,
            *mSwapchainFramebuffers[imageIndex],
            vk::Rect2D({0, 0}, mSwapchainExtent),
            0,
            nullptr};

        vk::ClearValue clearColor = vk::ClearValue().setColor(
            {mSwapColour.x, mSwapColour.y, mSwapColour.z, mSwapColour.w});

        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearColor;

        commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

        commandBuffer.endRenderPass();
        commandBuffer.end();
    }
    catch (const vk::SystemError& err)
    {
        mApplicationData->Console.LogError("Vulkan system error during command buffer recording: {}", err.what());
    }
    catch (const std::exception& ex)
    {
        mApplicationData->Console.LogError("Standard exception during command buffer recording: {}", ex.what());
    }
    catch (...)
    {
        mApplicationData->Console.LogError("Unknown error during command buffer recording!");
    }
}

void Mosaic::Renderer::SubmitCommandBuffer(vk::CommandBuffer& commandBuffer, std::uint32_t imageIndex)
{
    vk::SubmitInfo submitInfo = {};
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vk::Semaphore waitSemaphore = *mImageAvailableSemaphores[imageIndex];
    vk::Semaphore signalSemaphore = *mRenderFinishedSemaphores[imageIndex];

    vk::PipelineStageFlags mask = vk::PipelineStageFlagBits::eColorAttachmentOutput;

    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = &waitSemaphore;
    submitInfo.pWaitDstStageMask = &mask;

    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = &signalSemaphore;

    try
    {
        mGraphicsQueue.submit(submitInfo, nullptr);
    }
    catch (const vk::SystemError& error)
    {
        mApplicationData->Console.LogError("Failed to submit command buffer: {}", error.what());
    }
}

void Mosaic::Renderer::PresentImage(std::uint32_t imageIndex)
{
    vk::PresentInfoKHR presentInfo = {};
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = &*mRenderFinishedSemaphores[imageIndex];
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = &*mSwapchain;
    presentInfo.pImageIndices = &imageIndex;

    try
    {
        auto result = mGraphicsQueue.presentKHR(presentInfo);
    }
    catch (const vk::SystemError& error)
    {
        mApplicationData->Console.LogError("Failed to present swapchain image: {}", error.what());
    }
}

void Mosaic::Renderer::SelectSurfaceFormatAndPresentMode()
{
    auto formats = mPhysicalDevice.getSurfaceFormatsKHR(*mSurface);
    auto presentModes = mPhysicalDevice.getSurfacePresentModesKHR(*mSurface);
    auto capabilities = mPhysicalDevice.getSurfaceCapabilitiesKHR(*mSurface);

    if (formats.empty() or presentModes.empty())
    {
        mApplicationData->Console.LogError("Surface does not support any formats or present modes");
    }

    for (const auto& format : formats)
    {
        if (format.format == vk::Format::eB8G8R8A8Unorm &&
            format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
        {
            mSurfaceFormat = format;

            break;
        }
    }

    if (mSurfaceFormat.format == vk::Format::eUndefined)
    {
        mSurfaceFormat = formats[0];
    }

    if (std::ranges::find(presentModes, vk::PresentModeKHR::eMailbox) != presentModes.end())
    {
        mPresentMode = vk::PresentModeKHR::eMailbox;
    }
    else if (std::ranges::find(presentModes, vk::PresentModeKHR::eImmediate) != presentModes.end())
    {
        mPresentMode = vk::PresentModeKHR::eImmediate;
    }
    else
    {
        mPresentMode = vk::PresentModeKHR::eFifo;
    }

    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
    {
        mSwapchainExtent = capabilities.currentExtent;
    }
}

std::vector<const char*> Mosaic::Renderer::ExtractVectorStrings(const std::vector<std::string>& vector)
{
    std::vector<const char*> result;

    for (const auto& element : vector)
    {
        result.push_back(element.c_str());
    }

    return result;
}