#pragma once

#include <vulkan/vulkan.hpp>

#include <unordered_set>

namespace Mosaic
{
    class VulkanInstance;
    class VulkanQueues;

    class VulkanPhysicalDevice
    {
    public:
        void Select(VulkanInstance& instance);

        vk::PhysicalDevice& Get();

    private:
        vk::PhysicalDevice mPhysicalDevice;
    };

    class VulkanDevice
    {
    public:
        void GetExtensions(VulkanPhysicalDevice& physicalDevice, const std::unordered_set<std::string>& whitelist, const std::unordered_set<std::string>& blacklist);

        void Create(VulkanQueues& queues, VulkanPhysicalDevice& physicalDevice);

        vk::Device& Get();

    private:
        vk::UniqueDevice mDevice;

        std::vector<std::string> mExtensions;
    };
}