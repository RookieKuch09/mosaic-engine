#include "../../../include/rendering/vulkan/devices.hpp"
#include "../../../include/rendering/vulkan/instance.hpp"
#include "../../../include/rendering/vulkan/queues.hpp"

#include "../../../include/application/console.hpp"

void Mosaic::VulkanPhysicalDevice::Select(VulkanInstance& instance)
{
    auto result = instance.Get().enumeratePhysicalDevices();

    std::vector<vk::PhysicalDevice> physicalDevices;

    if (result.result != vk::Result::eSuccess)
    {
        Console::Throw("Error enumerating physical devices: {}", vk::to_string(result.result));
    }
    else
    {
        physicalDevices = std::move(result.value);
    }

    if (physicalDevices.empty())
    {
        Console::Throw("No Vulkan-compatible GPU located");
    }

    for (const auto& device : physicalDevices)
    {
        vk::PhysicalDeviceProperties properties = device.getProperties();

        bool discrete = properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu;
        bool integrated = properties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu;

        if (discrete or integrated)
        {
            mPhysicalDevice = device;

            break;
        }
    }

    if (not mPhysicalDevice)
    {
        Console::Throw("No available GPUs match requriements");
    }
}

vk::PhysicalDevice& Mosaic::VulkanPhysicalDevice::Get()
{
    return mPhysicalDevice;
}

void Mosaic::VulkanDevice::GetExtensions(VulkanPhysicalDevice& physicalDevice, const std::unordered_set<std::string>& whitelist, const std::unordered_set<std::string>& blacklist)
{
    std::vector<vk::ExtensionProperties> extensionProperties;

    auto result = physicalDevice.Get().enumerateDeviceExtensionProperties();

    if (result.result != vk::Result::eSuccess)
    {
        Console::Throw("Error retrieving vk::PhysicalDevice extensions: {}", vk::to_string(result.result));
    }

    extensionProperties = std::move(result.value);

    std::unordered_set<std::string> supportedExtensions;

    for (const auto& extension : extensionProperties)
    {
        supportedExtensions.insert(extension.extensionName);
    }

    mExtensions.clear();

    for (const auto& extension : supportedExtensions)
    {
        if (not blacklist.contains(extension))
        {
            mExtensions.push_back(extension);
        }
    }

    for (const auto& extension : whitelist)
    {
        if (blacklist.contains(extension))
        {
            Console::Throw("Whitelisted extension '{}' is also blacklisted — configuration error", extension);
        }

        if (not supportedExtensions.contains(extension))
        {
            Console::Throw("Requested whitelisted extension '{}' is not supported by the physical device", extension);
        }

        if (std::find(mExtensions.begin(), mExtensions.end(), extension) == mExtensions.end())
        {
            mExtensions.push_back(extension);
        }
    }
}

void Mosaic::VulkanDevice::Create(VulkanQueues& queues, VulkanPhysicalDevice& physicalDevice)
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

    auto extensions = getVectorCStrings(mExtensions);

    vk::DeviceCreateInfo deviceCreateInfo = {
        {},
        static_cast<std::uint32_t>(queues.GetQueueCreateInfo().size()),
        queues.GetQueueCreateInfo().data(),
        0,
        nullptr,
        static_cast<std::uint32_t>(extensions.size()),
        extensions.data(),
    };

    vk::PhysicalDeviceFeatures2 enabledFeatures{};
    vk::PhysicalDeviceVulkan11Features enabled11{};
    vk::PhysicalDeviceVulkan12Features enabled12{};
    vk::PhysicalDeviceVulkan13Features enabled13{};

    enabledFeatures.pNext = &enabled11;
    enabled11.pNext = &enabled12;
    enabled12.pNext = &enabled13;

    physicalDevice.Get().getFeatures2(&enabledFeatures);

    deviceCreateInfo.pNext = &enabledFeatures;

    auto result = physicalDevice.Get().createDeviceUnique(deviceCreateInfo);

    if (result.result != vk::Result::eSuccess)
    {
        Console::Throw("Error creating vk::Device: {}", vk::to_string(result.result));
    }
    else
    {
        mDevice = std::move(result.value);
    }
}

vk::Device& Mosaic::VulkanDevice::Get()
{
    return *mDevice;
}