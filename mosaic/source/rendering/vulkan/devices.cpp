#include "../../../include/rendering/vulkan/devices.hpp"
#include "../../../include/application/application.hpp"

void Mosaic::Backend::Vulkan::SelectPhysicalDevice(ApplicationData* applicationData, vk::UniqueInstance& instance, vk::PhysicalDevice& physicalDevice)
{
    std::vector<vk::PhysicalDevice> devices;

    try
    {
        devices = instance->enumeratePhysicalDevices();
    }
    catch (const vk::SystemError& error)
    {
        applicationData->Console.LogError("Error enumerating physical devices: {}", error.what());
    }

    if (devices.empty())
    {
        applicationData->Console.LogError("No suitable GPU found by Vulkan");
    }

    for (const auto& device : devices)
    {
        vk::PhysicalDeviceProperties props = device.getProperties();

        if (props.deviceType == vk::PhysicalDeviceType::eDiscreteGpu or props.deviceType == vk::PhysicalDeviceType::eIntegratedGpu)
        {
            physicalDevice = device;

            break;
        }
    }

    if (not physicalDevice)
    {
        applicationData->Console.LogError("No suitable discrete GPU found");
    }
}

void Mosaic::Backend::Vulkan::GetPhysicalDeviceExtensions(vk::PhysicalDevice& physicalDevice, std::vector<std::string>& extensions)
{
    std::vector<vk::ExtensionProperties> extensionProperties = physicalDevice.enumerateDeviceExtensionProperties();

    for (const auto& extension : extensionProperties)
    {
        std::string name = extension.extensionName;

        extensions.push_back(name);
    }
}

void Mosaic::Backend::Vulkan::CreateDevice(ApplicationData* applicationData, vk::Queue& graphicsQueue, vk::UniqueDevice& device, vk::PhysicalDevice& physicalDevice, const std::vector<std::string>& extensions)
{
    auto getVectorCStrings = [](const std::vector<std::string>& vector)
    {
        std::vector<const char*> result;

        for (const auto& element : vector)
        {
            result.push_back(element.c_str());
        }

        return result;
    };

    auto queueFamilies = physicalDevice.getQueueFamilyProperties();

    if (queueFamilies.empty())
    {
        applicationData->Console.LogError("No queue families found for the physical device");
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
        applicationData->Console.LogError("Failed to find graphics queue");
    }

    float queuePriority = 1.0;

    vk::DeviceQueueCreateInfo queueCreateInfo = {
        {},
        *graphicsQueueIndex,
        1,
        &queuePriority};

    auto rawExtensions = getVectorCStrings(extensions);

    vk::DeviceCreateInfo deviceCreateInfo = {
        {},
        1,
        &queueCreateInfo,
        0,
        nullptr,
        static_cast<uint32_t>(rawExtensions.size()),
        rawExtensions.data(),
    };

    try
    {
        device = physicalDevice.createDeviceUnique(deviceCreateInfo);
    }
    catch (const vk::SystemError& error)
    {
        applicationData->Console.LogError("Failed to create Vulkan device: {}", error.what());
    }

    graphicsQueue = device->getQueue(*graphicsQueueIndex, 0);

    if (not graphicsQueue)
    {
        applicationData->Console.LogError("Failed to get the graphics queue from the device");
    }
}