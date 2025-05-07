#include "../../../include/application/application.hpp"

#include "../../../include/rendering/vulkan/commands.hpp"
#include "../../../include/rendering/vulkan/devices.hpp"
#include "../../../include/rendering/vulkan/instance.hpp"
#include "../../../include/rendering/vulkan/renderer.hpp"
#include "../../../include/rendering/vulkan/swapchain.hpp"

#include <SDL3/SDL_vulkan.h>

Mosaic::VulkanRenderer::VulkanRenderer(ApplicationData* applicationData)
    : mApplicationData(applicationData)
{
}

void Mosaic::VulkanRenderer::Create()
{
    SDL_Vulkan_LoadLibrary(nullptr);

    CreateResources();
}

void Mosaic::VulkanRenderer::PreUpdate()
{
    AdvanceFrame();
    AwaitFrame();

    const auto imageIndex = AcquireFrame();
    if (not imageIndex.has_value())
    {
        return;
    }

    mImageIndex = imageIndex.value();

    Record();
}

void Mosaic::VulkanRenderer::PostUpdate()
{
    Submit();
    Present();
}

void Mosaic::VulkanRenderer::CreateResources()
{
    std::uint32_t count = 0;

    const char* const* rawRequestedExtensions = SDL_Vulkan_GetInstanceExtensions(&count);

    std::vector<std::string> requestedInstanceExtensions(rawRequestedExtensions, rawRequestedExtensions + count);
    std::vector<std::string> layers;
    std::vector<std::string> instanceExtensions;
    std::vector<std::string> deviceExtensions;

    GetLayers(
        mApplicationData,
        layers,
        {});

    GetInstanceExtensions(
        mApplicationData,
        instanceExtensions,
        requestedInstanceExtensions);

    CreateInstance(
        mApplicationData,
        mInstance,
        layers,
        instanceExtensions);

    SelectPhysicalDevice(
        mApplicationData,
        mInstance,
        mPhysicalDevice);

    GetPhysicalDeviceExtensions(
        mPhysicalDevice,
        deviceExtensions);

    CreateDevice(
        mApplicationData,
        mGraphicsQueue,
        mDevice,
        mPhysicalDevice,
        mGraphicsQueueFamilyIndex,
        deviceExtensions);

    VkSurfaceKHR rawSurface;

    if (not SDL_Vulkan_CreateSurface(mApplicationData->Window.mHandle, *mInstance, nullptr, &rawSurface))
    {
        mApplicationData->Console.LogError("Failed to create Vulkan surface");
    }

    mSurface = vk::UniqueSurfaceKHR(rawSurface, *mInstance);

    GetSwapchainData(
        mApplicationData,
        mPhysicalDevice,
        mSurface,
        mSurfaceFormat,
        mSwapchainExtent,
        vk::PresentModeKHR::eFifo,
        mPresentMode,
        mImageCount);

    CreateSwapchain(
        mApplicationData,
        mDevice,
        mSurface,
        mSurfaceFormat,
        mSwapchainExtent,
        mPresentMode,
        mSwapchain);

    mSwapchainImages = mDevice->getSwapchainImagesKHR(*mSwapchain);

    mSwapchainImageViews.reserve(mSwapchainImages.size());
    mSwapchainFramebuffers.reserve(mSwapchainImages.size());

    CreateRenderPass(
        mApplicationData,
        mDevice,
        mSurfaceFormat,
        mRenderPass);

    CreateSwapchainImages(
        mApplicationData,
        mDevice,
        mSurfaceFormat,
        mSwapchainImages,
        mSwapchainImageViews);

    CreateFramebuffers(
        mApplicationData,
        mDevice,
        mSurfaceFormat,
        mRenderPass,
        mSwapchainExtent,
        mSwapchainFramebuffers,
        mSwapchainImageViews);

    CreateCommandResources(
        mApplicationData,
        mDevice,
        mCommandPool,
        mCommandBuffers,
        mSwapchainFramebuffers.size());

    vk::SemaphoreCreateInfo semaphoreInfo = {};
    vk::FenceCreateInfo fenceInfo = {vk::FenceCreateFlagBits::eSignaled};

    size_t frames = mSwapchainImages.size();

    mImageAvailableSemaphores.resize(frames);
    mRenderFinishedSemaphores.resize(frames);
    mInFlightFences.resize(frames);

    for (std::uint32_t index = 0; index < frames; index++)
    {
        mImageAvailableSemaphores[index] = mDevice->createSemaphoreUnique(semaphoreInfo);
        mRenderFinishedSemaphores[index] = mDevice->createSemaphoreUnique(semaphoreInfo);
        mInFlightFences[index] = mDevice->createFenceUnique(fenceInfo);
    }
}

void Mosaic::VulkanRenderer::Record()
{
    if (not mRenderPass)
    {
        mApplicationData->Console.LogError("Render pass is null!");
    }

    if (mImageIndex >= mSwapchainFramebuffers.size())
    {
        mApplicationData->Console.LogError("Invalid image index: {} (framebuffers size: {})", mImageIndex, mSwapchainFramebuffers.size());
    }

    if (not mSwapchainFramebuffers[mImageIndex])
    {
        mApplicationData->Console.LogError("Framebuffer for image {} is null!", mImageIndex);
    }

    if (mSwapchainExtent.width == 0 or mSwapchainExtent.height == 0)
    {
        mApplicationData->Console.LogError("Swapchain extent is invalid: {}x{}", mSwapchainExtent.width, mSwapchainExtent.height);
    }

    auto& commandBuffer = mCommandBuffers[mImageIndex];

    try
    {
        vk::CommandBufferBeginInfo beginInfo = {};
        commandBuffer->begin(beginInfo);

        vk::RenderPassBeginInfo renderPassBeginInfo = {
            *mRenderPass,
            *mSwapchainFramebuffers[mImageIndex],
            vk::Rect2D({0, 0}, mSwapchainExtent),
            0,
            nullptr};

        vk::ClearValue clearColor = vk::ClearValue().setColor({mClearColour.x, mClearColour.y, mClearColour.z, mClearColour.w});

        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearColor;

        commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
    }
    catch (const vk::SystemError& error)
    {
        mApplicationData->Console.LogError("Vulkan system error during command buffer recording: {}", error.what());
    }
    catch (const std::exception& exception)
    {
        mApplicationData->Console.LogError("Standard exception during command buffer recording: {}", exception.what());
    }
    catch (...)
    {
        mApplicationData->Console.LogError("Unknown error during command buffer recording!");
    }
}

void Mosaic::VulkanRenderer::Submit()
{
    auto& commandBuffer = mCommandBuffers[mImageIndex];

    commandBuffer->endRenderPass();
    commandBuffer->end();

    vk::Semaphore waitSemaphores[] = {*mImageAvailableSemaphores[mFrameIndex]};
    vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};
    vk::Semaphore signalSemaphores[] = {*mRenderFinishedSemaphores[mFrameIndex]};

    vk::SubmitInfo submitInfo(
        1,
        waitSemaphores,
        waitStages,
        1,
        &*commandBuffer,
        1,
        signalSemaphores);

    try
    {
        mGraphicsQueue.submit(submitInfo, *mInFlightFences[mFrameIndex]);
    }
    catch (const vk::SystemError& error)
    {
        mApplicationData->Console.LogError("Failed to submit command buffer: {}", error.what());
    }
}

void Mosaic::VulkanRenderer::Present()
{
    vk::PresentInfoKHR presentInfo(
        1,
        &*mRenderFinishedSemaphores[mFrameIndex],
        1,
        &*mSwapchain,
        &mImageIndex,
        nullptr);

    try
    {
        auto result = mGraphicsQueue.presentKHR(presentInfo);

        if (result == vk::Result::eErrorOutOfDateKHR or result == vk::Result::eSuboptimalKHR)
        {
        }
    }
    catch (const vk::SystemError& error)
    {
        mApplicationData->Console.LogError("Failed to present swapchain image: {}", error.what());
    }
}

void Mosaic::VulkanRenderer::AwaitFrame()
{
    mDevice->waitForFences(1, &*mInFlightFences[mFrameIndex], true, std::numeric_limits<std::uint64_t>::max());
    mDevice->resetFences(1, &*mInFlightFences[mFrameIndex]);
}

std::optional<std::uint32_t> Mosaic::VulkanRenderer::AcquireFrame()
{
    uint32_t mImageIndex;

    vk::Result result = mDevice->acquireNextImageKHR(
        *mSwapchain,
        std::numeric_limits<std::uint64_t>::max(),
        *mImageAvailableSemaphores[mFrameIndex],
        nullptr,
        &mImageIndex);

    if (result == vk::Result::eErrorOutOfDateKHR or result == vk::Result::eSuboptimalKHR)
    {
        return std::nullopt;
    }

    if (result != vk::Result::eSuccess)
    {
        mApplicationData->Console.LogError("Failed to acquire swapchain image: {}", vk::to_string(result));

        return std::nullopt;
    }

    return mImageIndex;
}

void Mosaic::VulkanRenderer::AdvanceFrame()
{
    mFrameIndex = (mFrameIndex + 1) % static_cast<std::uint32_t>(mInFlightFences.size());
}
