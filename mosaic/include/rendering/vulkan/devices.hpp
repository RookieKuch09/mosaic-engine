#pragma once

#include <vulkan/vulkan.hpp>

namespace Mosaic
{
    struct ApplicationData;
}

namespace Mosaic::Backend::Vulkan
{
    void SelectPhysicalDevice(
        ApplicationData* applicationData,
        vk::UniqueInstance& instance,
        vk::PhysicalDevice& physicalDevice);

    void GetPhysicalDeviceExtensions(
        vk::PhysicalDevice& physicalDevice,
        std::vector<std::string>& extensions);

    void CreateDevice(
        ApplicationData* applicationData,
        vk::Queue& graphicsQueue,
        vk::UniqueDevice& device,
        vk::PhysicalDevice& physicalDevice,
        std::uint32_t graphicsQueueFamilyIndex,
        const std::vector<std::string>& extensions);
}