#include "../../../include/application/application.hpp"
#include "../../../include/application/console.hpp"

#include "../../../include/rendering/vulkan/commands.hpp"
#include "../../../include/rendering/vulkan/devices.hpp"
#include "../../../include/rendering/vulkan/instance.hpp"
#include "../../../include/rendering/vulkan/renderer.hpp"
#include "../../../include/rendering/vulkan/swapchain.hpp"

#include "../../../include/utilities/config.hpp"

#include <SDL3/SDL_vulkan.h>

Mosaic::VulkanRenderer::VulkanRenderer(ApplicationData* applicationData)
    : mApplicationData(applicationData), mRebuildSwapchain(false)
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

    if (mRebuildSwapchain)
    {
        DestroySwapchain();
        RebuildSwapchain();

        mRebuildSwapchain = false;
    }

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
    if (not mRebuildSwapchain)
    {
        Submit();
        Present();
    }
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
        layers,
        {});

    GetInstanceExtensions(
        instanceExtensions,
        requestedInstanceExtensions);

    CreateInstance(
        mInstance,
        layers,
        instanceExtensions);

    SelectPhysicalDevice(
        mInstance,
        mPhysicalDevice);

    GetPhysicalDeviceExtensions(
        mPhysicalDevice,
        deviceExtensions);

    CreateDevice(
        mGraphicsQueue,
        mDevice,
        mPhysicalDevice,
        mGraphicsQueueFamilyIndex,
        deviceExtensions);

    VkSurfaceKHR rawSurface;

    if (not SDL_Vulkan_CreateSurface(mApplicationData->Window.mHandle, *mInstance, nullptr, &rawSurface))
    {
        Console::Throw("Failed to create Vulkan surface");
    }

    mSurface = vk::UniqueSurfaceKHR(rawSurface, *mInstance);

    GetSwapchainData(
        mPhysicalDevice,
        mSurface,
        mSurfaceFormat,
        mSwapchainExtent,
        mVSync,
        mPresentMode,
        mApplicationData->Window.GetSize(),
        mImageCount);

    CreateSwapchain(
        mDevice,
        mSurface,
        mSurfaceFormat,
        mSwapchainExtent,
        mImageCount,
        mPresentMode,
        mSwapchain);

    mSwapchainImages = mDevice->getSwapchainImagesKHR(*mSwapchain);

    mSwapchainImageViews.reserve(mSwapchainImages.size());
    mSwapchainFramebuffers.reserve(mSwapchainImages.size());

    CreateRenderPass(
        mDevice,
        mSurfaceFormat,
        mRenderPass);

    CreateSwapchainImages(
        mDevice,
        mSurfaceFormat,
        mSwapchainImages,
        mSwapchainImageViews);

    CreateFramebuffers(
        mDevice,
        mSurfaceFormat,
        mRenderPass,
        mSwapchainExtent,
        mSwapchainFramebuffers,
        mSwapchainImageViews);

    CreateCommandResources(
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

void Mosaic::VulkanRenderer::DestroySwapchain()
{
    mDevice->waitIdle();

    mSwapchain.release();
    mCommandPool.release();
    mRenderPass.release();

    mSwapchainImages.clear();
    mSwapchainImageViews.clear();
    mSwapchainFramebuffers.clear();
    mImageAvailableSemaphores.clear();
    mRenderFinishedSemaphores.clear();
    mInFlightFences.clear();
}

void Mosaic::VulkanRenderer::RebuildSwapchain()
{
    mSwapchainImages = mDevice->getSwapchainImagesKHR(*mSwapchain);

    std::uint32_t swapchainSize = mSwapchainImages.size();

    mSwapchainImageViews.reserve(swapchainSize);
    mSwapchainFramebuffers.reserve(swapchainSize);
    mImageAvailableSemaphores.resize(swapchainSize);
    mRenderFinishedSemaphores.resize(swapchainSize);
    mInFlightFences.resize(swapchainSize);

    vk::SemaphoreCreateInfo semaphoreInfo = {};
    vk::FenceCreateInfo fenceInfo = {vk::FenceCreateFlagBits::eSignaled};

    for (std::uint32_t index = 0; index < swapchainSize; index++)
    {
        mImageAvailableSemaphores[index] = mDevice->createSemaphoreUnique(semaphoreInfo);
        mRenderFinishedSemaphores[index] = mDevice->createSemaphoreUnique(semaphoreInfo);
        mInFlightFences[index] = mDevice->createFenceUnique(fenceInfo);
    }

    GetSwapchainData(
        mPhysicalDevice,
        mSurface,
        mSurfaceFormat,
        mSwapchainExtent,
        mVSync,
        mPresentMode,
        mApplicationData->Window.GetSize(),
        mImageCount);

    CreateSwapchain(
        mDevice,
        mSurface,
        mSurfaceFormat,
        mSwapchainExtent,
        mImageCount,
        mPresentMode,
        mSwapchain);

    mSwapchainImages = mDevice->getSwapchainImagesKHR(*mSwapchain);

    mSwapchainImageViews.reserve(mSwapchainImages.size());
    mSwapchainFramebuffers.reserve(mSwapchainImages.size());

    CreateRenderPass(
        mDevice,
        mSurfaceFormat,
        mRenderPass);

    CreateSwapchainImages(
        mDevice,
        mSurfaceFormat,
        mSwapchainImages,
        mSwapchainImageViews);

    CreateFramebuffers(
        mDevice,
        mSurfaceFormat,
        mRenderPass,
        mSwapchainExtent,
        mSwapchainFramebuffers,
        mSwapchainImageViews);

    CreateCommandResources(
        mDevice,
        mCommandPool,
        mCommandBuffers,
        mSwapchainFramebuffers.size());

    mFrameIndex = 0;
    mImageIndex = 0;
}

void Mosaic::VulkanRenderer::Record()
{
    if (not mRenderPass)
    {
        Console::Throw("Render pass is null!");
    }

    if (mImageIndex >= mSwapchainFramebuffers.size())
    {
        Console::Throw("Invalid image index: {} (framebuffers size: {})", mImageIndex, mSwapchainFramebuffers.size());
    }

    if (not mSwapchainFramebuffers[mImageIndex])
    {
        Console::Throw("Framebuffer for image {} is null!", mImageIndex);
    }

    if (mSwapchainExtent.width == 0 or mSwapchainExtent.height == 0)
    {
        Console::Throw("Swapchain extent is invalid: {}x{}", mSwapchainExtent.width, mSwapchainExtent.height);
    }

    auto& commandBuffer = mCommandBuffers[mImageIndex];

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

    mGraphicsQueue.submit(submitInfo, *mInFlightFences[mFrameIndex]);
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
        mGraphicsQueue.presentKHR(presentInfo);
    }
    catch (const vk::SystemError& error)
    {
        vk::Result result = static_cast<vk::Result>(error.code().value());

        if (result == vk::Result::eErrorOutOfDateKHR or result == vk::Result::eSuboptimalKHR)
        {
            mRebuildSwapchain = true;
        }
    }
}

void Mosaic::VulkanRenderer::AwaitFrame()
{
    vk::Result result;

    result = mDevice->waitForFences(1, &*mInFlightFences[mFrameIndex], true, std::numeric_limits<std::uint64_t>::max());

    if (result != vk::Result::eSuccess)
    {
        std::string name = vk::to_string(result);

        Console::Throw("Vulkan exception: {}", name);
    }

    result = mDevice->resetFences(1, &*mInFlightFences[mFrameIndex]);

    if (result != vk::Result::eSuccess)
    {
        std::string name = vk::to_string(result);

        Console::Throw("Vulkan exception: {}", name);
    }
}

std::optional<std::uint32_t> Mosaic::VulkanRenderer::AcquireFrame()
{
    try
    {
        mDevice->acquireNextImageKHR(
            *mSwapchain,
            std::numeric_limits<std::uint64_t>::max(),
            *mImageAvailableSemaphores[mFrameIndex],
            nullptr,
            &mImageIndex);
    }
    catch (const vk::SystemError& error)
    {
        vk::Result result = static_cast<vk::Result>(error.code().value());

        if (result == vk::Result::eErrorOutOfDateKHR or result == vk::Result::eSuboptimalKHR)
        {
            mRebuildSwapchain = true;

            return std::nullopt;
        }
        else if (result != vk::Result::eSuccess)
        {
            Console::Throw("Failed to acquire swapchain image: {}", vk::to_string(result));

            return std::nullopt;
        }
    }

    return mImageIndex;
}

void Mosaic::VulkanRenderer::AdvanceFrame()
{
    mFrameIndex = (mFrameIndex + 1) % static_cast<std::uint32_t>(mInFlightFences.size());
}

void Mosaic::VulkanRenderer::LoadConfig()
{
    TOMLFile file(mConfigPath);
}