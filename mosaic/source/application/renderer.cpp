#include "../../include/application/application.hpp"

#include <SDL3/SDL_vulkan.h>

Mosaic::Renderer::Renderer(ApplicationData& applicationData)
    : mApplicationData(&applicationData)
{
}

void Mosaic::Renderer::Create()
{
    SDL_Vulkan_LoadLibrary(nullptr);

    CreateInstance();
    CreatePhysicalDevice();
    CreateGraphicsQueueAndDevice();
    CreateSurface();
    CreateSwapchain();
    CreateRenderPass();
    CreateFramebuffers();
    CreateCommandBuffers();
    CreateSemaphores();
}

void Mosaic::Renderer::Update()
{
    uint32_t imageIndex = 0;
    vk::Result result = mDevice->acquireNextImageKHR(
        *mSwapchain,
        100000000, nullptr,
        nullptr,
        &imageIndex);

    LogEvent("Acquired image");

    if (result == vk::Result::eErrorOutOfDateKHR)
    {
    }

    RecordCommandBuffer(*(mCommandBuffers[imageIndex]), imageIndex);
    SubmitCommandBuffer(*(mCommandBuffers[imageIndex]), imageIndex);

    PresentImage(imageIndex);

    LogEvent("Updated");
}

void Mosaic::Renderer::GetBaseExtensions()
{
    unsigned int count = 0;
    const char* const* extensions = SDL_Vulkan_GetInstanceExtensions(&count);

    if (count == 0 or extensions == nullptr)
    {
        LogError("Failed to get Vulkan instance extensions");
    }
    else
    {
        for (unsigned int index = 0; index < count; index++)
        {
            mBaseExtensions.push_back(extensions[index]);
        }
    }
}

void Mosaic::Renderer::GetDeviceExtensions()
{
    std::vector<vk::ExtensionProperties> extensionProperties = mPhysicalDevice.enumerateDeviceExtensionProperties();

    for (const auto& extension : extensionProperties)
    {
        std::string name = extension.extensionName;

        mDeviceExtensions.push_back(name);
    }
}

void Mosaic::Renderer::CreateInstance()
{
    GetBaseExtensions();

    vk::ApplicationInfo appInfo = {
        "-",
        VK_MAKE_VERSION(0, 0, 0),
        "Mosaic",
        VK_MAKE_VERSION(0, 0, 0),
        VK_API_VERSION_1_4,
    };

    if (mApplicationData->DebugMode)
    {
        mLayers.push_back("VK_LAYER_KHRONOS_validation");
    }

    auto layers = ExtractVectorStrings(mLayers);
    auto extensions = ExtractVectorStrings(mBaseExtensions);

    vk::InstanceCreateInfo instanceInfo = {
        {},
        &appInfo,
        static_cast<unsigned int>(mLayers.size()),
        layers.data(),
        static_cast<unsigned int>(mBaseExtensions.size()),
        extensions.data()};

    try
    {
        mInstance = vk::createInstanceUnique(instanceInfo);
    }
    catch (const vk::SystemError& error)
    {
        LogError("Failed to create Vulkan instance: {}", error.what());
    }

    if (not mInstance)
    {
        LogError("Failed to create Vulkan instance, unknown error");
    }

    LogEvent("Instance created");
}

void Mosaic::Renderer::CreatePhysicalDevice()
{
    std::vector<vk::PhysicalDevice> devices;

    try
    {
        devices = mInstance->enumeratePhysicalDevices();
    }
    catch (const vk::SystemError& error)
    {
        LogError("Error enumerating physical devices: {}", error.what());
    }

    if (devices.empty())
    {
        LogError("No suitable GPU found by Vulkan");
    }

    for (const auto& device : devices)
    {
        vk::PhysicalDeviceProperties props = device.getProperties();
        vk::PhysicalDeviceFeatures features = device.getFeatures();

        if (props.deviceType == vk::PhysicalDeviceType::eDiscreteGpu)
        {
            mPhysicalDevice = device;

            break;
        }
    }

    if (not mPhysicalDevice)
    {
        LogError("No suitable discrete GPU found");
    }

    GetDeviceExtensions();

    LogEvent("PDevice created");
}

void Mosaic::Renderer::CreateGraphicsQueueAndDevice()
{
    auto queueFamilies = mPhysicalDevice.getQueueFamilyProperties();

    if (queueFamilies.empty())
    {
        LogError("No queue families found for the physical device");
    }

    std::optional<unsigned int> graphicsQueueIndex;

    for (int index = 0; index < queueFamilies.size(); index++)
    {
        if (queueFamilies[index].queueFlags & vk::QueueFlagBits::eGraphics)
        {
            graphicsQueueIndex = index;

            break;
        }
    }

    if (not graphicsQueueIndex)
    {
        LogError("Failed to find graphics queue");
    }

    float queuePriority = 1.0;

    vk::DeviceQueueCreateInfo queueCreateInfo = {
        {},
        *graphicsQueueIndex,
        1,
        &queuePriority};

    auto layers = ExtractVectorStrings(mLayers);
    auto extensions = ExtractVectorStrings(mDeviceExtensions);

    vk::DeviceCreateInfo deviceCreateInfo = {
        {},
        1,
        &queueCreateInfo,
        static_cast<uint32_t>(mLayers.size()),
        layers.data(),
        static_cast<uint32_t>(mDeviceExtensions.size()),
        extensions.data(),
    };

    try
    {
        mDevice = mPhysicalDevice.createDeviceUnique(deviceCreateInfo);
    }
    catch (const vk::SystemError& error)
    {
        LogError("Failed to create Vulkan device: {}", error.what());
    }

    mGraphicsQueue = mDevice->getQueue(*graphicsQueueIndex, 0);

    if (not mGraphicsQueue)
    {
        LogError("Failed to get the graphics queue from the device");
    }

    LogEvent("GQueue and Device created");
}

void Mosaic::Renderer::CreateSurface()
{
    VkSurfaceKHR rawSurface;

    if (!SDL_Vulkan_CreateSurface(mApplicationData->Window.mHandle, *mInstance, nullptr, &rawSurface))
    {
        LogError("Failed to create Vulkan surface");
    }

    mSurface = vk::UniqueSurfaceKHR(rawSurface, *mInstance);

    LogEvent("Surface created");
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
        LogError("Failed to create Vulkan swapchain: {}", error.what());
    }

    if (not mSwapchain)
    {
        LogError("Failed to create swapchain (unknown error)");
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
            LogError("Failed to create image view for swapchain image: {}", error.what());
        }
    }

    mImageCount = mSwapchainImages.size();

    LogEvent("Swapchain created");
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
        LogError("Failed to create render pass: {}", error.what());
    }

    LogEvent("Renderpass created");
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
            LogError("Failed to create framebuffer image: {}", error.what());
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
            LogError("Failed to create framebuffer: {}", error.what());
        }
    }

    LogEvent("Framebuffers created");
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
        LogError("Failed to create command buffers: {}", error.what());
    }

    LogEvent("CommandBuffers created");
}

void Mosaic::Renderer::CreateSemaphores()
{
    vk::SemaphoreCreateInfo semaphoreCreateInfo = {};

    mImageAvailableSemaphores.resize(mSwapchainImages.size());
    mRenderFinishedSemaphores.resize(mSwapchainImages.size());

    for (size_t i = 0; i < mSwapchainImages.size(); ++i)
    {
        mImageAvailableSemaphores[i] = mDevice->createSemaphoreUnique(semaphoreCreateInfo);
        mRenderFinishedSemaphores[i] = mDevice->createSemaphoreUnique(semaphoreCreateInfo);
    }

    LogEvent("Semaphores created");
}

void Mosaic::Renderer::RecordCommandBuffer(vk::CommandBuffer& commandBuffer, std::uint32_t imageIndex)
{
    LogEvent("Recording command buffer for image {}", imageIndex);

    // Sanity checks
    if (!mRenderPass)
    {
        LogError("Render pass is null!");
        return;
    }

    if (imageIndex >= mSwapchainFramebuffers.size())
    {
        LogError("Invalid image index: {} (framebuffers size: {})", imageIndex, mSwapchainFramebuffers.size());
        return;
    }

    if (!mSwapchainFramebuffers[imageIndex])
    {
        LogError("Framebuffer for image {} is null!", imageIndex);
        return;
    }

    if (mSwapchainExtent.width == 0 || mSwapchainExtent.height == 0)
    {
        LogError("Swapchain extent is invalid: {}x{}", mSwapchainExtent.width, mSwapchainExtent.height);
        return;
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

        LogEvent("Command buffer recorded successfully for image {}", imageIndex);
    }
    catch (const vk::SystemError& err)
    {
        LogError("Vulkan system error during command buffer recording: {}", err.what());
    }
    catch (const std::exception& ex)
    {
        LogError("Standard exception during command buffer recording: {}", ex.what());
    }
    catch (...)
    {
        LogError("Unknown error during command buffer recording!");
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
        LogError("Failed to submit command buffer: {}", error.what());
    }

    LogEvent("Command buffer submitted");
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
        LogError("Failed to present swapchain image: {}", error.what());
    }

    LogEvent("Presented");
}

void Mosaic::Renderer::SelectSurfaceFormatAndPresentMode()
{
    auto formats = mPhysicalDevice.getSurfaceFormatsKHR(*mSurface);
    auto presentModes = mPhysicalDevice.getSurfacePresentModesKHR(*mSurface);
    auto capabilities = mPhysicalDevice.getSurfaceCapabilitiesKHR(*mSurface);

    if (formats.empty() or presentModes.empty())
    {
        LogError("Surface does not support any formats or present modes");
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