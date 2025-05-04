#include "../../include/application/renderer.hpp"
#include "../../include/application/application.hpp"
#include "../../include/application/logging.hpp"

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
}

void Mosaic::Renderer::Update()
{
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
}

void Mosaic::Renderer::CreateSurface()
{
    VkSurfaceKHR rawSurface;
    if (!SDL_Vulkan_CreateSurface(mApplicationData->Window.mHandle, *mInstance, nullptr, &rawSurface))
    {
        LogError("Failed to create Vulkan surface");
    }

    mSurface = vk::UniqueSurfaceKHR(rawSurface, *mInstance);
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