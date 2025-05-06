#include "../../include/application/application.hpp"

#include "../../include/rendering/vulkan/commands.hpp"
#include "../../include/rendering/vulkan/devices.hpp"
#include "../../include/rendering/vulkan/instance.hpp"
#include "../../include/rendering/vulkan/swapchain.hpp"

#include <SDL3/SDL_vulkan.h>

Mosaic::Renderer::Renderer(ApplicationData& applicationData)
    : mImageCount(0), mImageIndex(0), mFrameIndex(0), mApplicationData(&applicationData)
{
}

void Mosaic::Renderer::Create()
{
    SDL_Vulkan_LoadLibrary(nullptr);

    CreateInstance();
    CreateDevices();
    CreateSurface();
    CreateSwapchain();
    CreateCommandBuffers();
    CreateSyncObjects();
}

void Mosaic::Renderer::FirstUpdate()
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

void Mosaic::Renderer::SecondUpdate()
{
    Submit();
    Present();
}

void Mosaic::Renderer::CreateInstance()
{
    std::uint32_t count = 0;

    const char* const* rawRequestedExtensions = SDL_Vulkan_GetInstanceExtensions(&count);

    std::vector<std::string> requestedExtensions(rawRequestedExtensions, rawRequestedExtensions + count);
    std::vector<std::string> layers;
    std::vector<std::string> extensions;

    Backend::Vulkan::GetLayers(
        mApplicationData,
        layers,
        {});

    Backend::Vulkan::GetInstanceExtensions(
        mApplicationData,
        extensions,
        requestedExtensions);

    Backend::Vulkan::CreateInstance(
        mApplicationData,
        mInstance,
        layers,
        extensions);
}

void Mosaic::Renderer::CreateDevices()
{
    std::vector<std::string> extensions;

    Backend::Vulkan::SelectPhysicalDevice(
        mApplicationData,
        mInstance,
        mPhysicalDevice);

    Backend::Vulkan::GetPhysicalDeviceExtensions(
        mPhysicalDevice,
        extensions);

    Backend::Vulkan::CreateDevice(
        mApplicationData,
        mGraphicsQueue,
        mDevice,
        mPhysicalDevice,
        mGraphicsQueueFamilyIndex,
        extensions);
}

void Mosaic::Renderer::CreateSurface()
{
    VkSurfaceKHR rawSurface;

    if (not SDL_Vulkan_CreateSurface(mApplicationData->Window.mHandle, *mInstance, nullptr, &rawSurface))
    {
        mApplicationData->Console.LogError("Failed to create Vulkan surface");
    }

    mSurface = vk::UniqueSurfaceKHR(rawSurface, *mInstance);
}

void Mosaic::Renderer::CreateSwapchain()
{
    Backend::Vulkan::GetSwapchainData(
        mApplicationData,
        mPhysicalDevice,
        mSurface,
        mSurfaceFormat,
        mSwapchainExtent,
        vk::PresentModeKHR::eImmediate,
        mPresentMode,
        mImageCount);

    Backend::Vulkan::CreateSwapchain(
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

    Backend::Vulkan::CreateRenderPass(
        mApplicationData,
        mDevice,
        mSurfaceFormat,
        mRenderPass);

    Backend::Vulkan::CreateSwapchainImages(
        mApplicationData,
        mDevice,
        mSurfaceFormat,
        mSwapchainImages,
        mSwapchainImageViews);

    Backend::Vulkan::CreateFramebuffers(
        mApplicationData,
        mDevice,
        mSurfaceFormat,
        mRenderPass,
        mSwapchainExtent,
        mSwapchainFramebuffers,
        mSwapchainImageViews);
}

void Mosaic::Renderer::CreateCommandBuffers()
{
    Backend::Vulkan::CreateCommandResources(
        mApplicationData,
        mDevice,
        mCommandPool,
        mCommandBuffers,
        mSwapchainFramebuffers.size());
}

void Mosaic::Renderer::CreateSyncObjects()
{
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

void Mosaic::Renderer::Record()
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

        vk::ClearValue clearColor = vk::ClearValue().setColor({mSwapColour.x, mSwapColour.y, mSwapColour.z, mSwapColour.w});

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

void Mosaic::Renderer::Submit()
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

void Mosaic::Renderer::Present()
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

void Mosaic::Renderer::AwaitFrame()
{
    mDevice->waitForFences(1, &*mInFlightFences[mFrameIndex], true, std::numeric_limits<std::uint64_t>::max());
    mDevice->resetFences(1, &*mInFlightFences[mFrameIndex]);
}

std::optional<std::uint32_t> Mosaic::Renderer::AcquireFrame()
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

void Mosaic::Renderer::AdvanceFrame()
{
    mFrameIndex = (mFrameIndex + 1) % static_cast<uint32_t>(mInFlightFences.size());
}
