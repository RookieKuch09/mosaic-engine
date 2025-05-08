#include "../../../include/rendering/vulkan/devices.hpp"
#include "../../../include/application/console.hpp"

void Mosaic::SelectPhysicalDevice(
    vk::UniqueInstance& instance,
    vk::PhysicalDevice& physicalDevice)
{
    std::vector<vk::PhysicalDevice> devices;

    devices = instance->enumeratePhysicalDevices();

    if (devices.empty())
    {
        Console::Throw("No suitable GPU found by Vulkan");
    }

    for (const auto& device : devices)
    {
        vk::PhysicalDeviceProperties props = device.getProperties();

        if (props.deviceType == vk::PhysicalDeviceType::eDiscreteGpu or props.deviceType == vk::PhysicalDeviceType::eIntegratedGpu or props.deviceType == vk::PhysicalDeviceType::eVirtualGpu)
        {
            physicalDevice = device;

            break;
        }
    }

    if (not physicalDevice)
    {
        Console::Throw("No suitable GPU found");
    }
}

void Mosaic::GetPhysicalDeviceExtensions(
    vk::PhysicalDevice& physicalDevice,
    std::vector<std::string>& extensions)
{
    std::vector<vk::ExtensionProperties> extensionProperties = physicalDevice.enumerateDeviceExtensionProperties();

    for (const auto& extension : extensionProperties)
    {
        std::string name = extension.extensionName;

        extensions.push_back(name);
    }
}

void Mosaic::CreateDevice(
    vk::Queue& graphicsQueue,
    vk::UniqueDevice& device,
    vk::PhysicalDevice& physicalDevice,
    std::uint32_t& graphicsQueueFamilyIndex,
    const std::vector<std::string>& extensions)
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
        Console::Throw("No queue families found for the physical device");
    }

    std::optional<std::uint32_t> graphicsQueueIndex;

    for (std::int32_t index = 0; index < queueFamilies.size(); index++)
    {
        if (queueFamilies[index].queueFlags bitand vk::QueueFlagBits::eGraphics)
        {
            graphicsQueueIndex = index;

            break;
        }
    }

    if (not graphicsQueueIndex)
    {
        Console::Throw("Failed to find graphics queue");
    }

    graphicsQueueFamilyIndex = graphicsQueueIndex.value();

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
        static_cast<std::uint32_t>(rawExtensions.size()),
        rawExtensions.data(),
    };

    vk::PhysicalDeviceFeatures2 enabledFeatures{};
    vk::PhysicalDeviceVulkan11Features enabled11{};
    vk::PhysicalDeviceVulkan12Features enabled12{};
    vk::PhysicalDeviceVulkan13Features enabled13{};

    enabledFeatures.pNext = &enabled11;
    enabled11.pNext = &enabled12;
    enabled12.pNext = &enabled13;

    physicalDevice.getFeatures2(&enabledFeatures);

    deviceCreateInfo.pNext = &enabledFeatures;

    device = physicalDevice.createDeviceUnique(deviceCreateInfo);

    graphicsQueue = device->getQueue(*graphicsQueueIndex, 0);

    if (not graphicsQueue)
    {
        Console::Throw("Failed to get the graphics queue from the device");
    }
}