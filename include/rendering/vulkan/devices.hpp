#pragma once

#include "utilities/numerics.hpp"

#include <vulkan/vulkan.hpp>

#include <unordered_set>

namespace Mosaic::Internal::Rendering
{
    class VulkanInstance;
    class VulkanQueues;
    class VulkanSwapchain;
    class VulkanRenderer;

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

        void AwaitFences(VulkanSwapchain& swapchain);

        Types::UInt32 GetNextImageIndex(VulkanRenderer& renderer, VulkanSwapchain& swapchain);

        void WaitIdle();

        vk::Device& Get();

    private:
        vk::UniqueDevice mDevice;

        std::vector<std::string> mExtensions;
    };
}