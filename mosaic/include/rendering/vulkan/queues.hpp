#pragma once

#include <vulkan/vulkan.hpp>

namespace Mosaic
{
    class VulkanPhysicalDevice;
    class VulkanDevice;
    class VulkanSurface;

    class VulkanQueues
    {
    public:
        void Discover(VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface);

        void Load(VulkanDevice& device);

        vk::Queue GetGraphicsQueue() const;
        vk::Queue GetComputeQueue() const;
        vk::Queue GetTransferQueue() const;
        vk::Queue GetPresentQueue() const;

        std::uint32_t GetGraphicsQueueFamily() const;
        std::uint32_t GetComputeQueueFamily() const;
        std::uint32_t GetTransferQueueFamily() const;
        std::uint32_t GetPresentQueueFamily() const;

        std::vector<vk::DeviceQueueCreateInfo>& GetQueueCreateInfo();

    private:
        void ResolveFallbacks();

        vk::Queue mGraphicsQueue;
        vk::Queue mComputeQueue;
        vk::Queue mTransferQueue;
        vk::Queue mPresentQueue;

        std::optional<std::uint32_t> mGraphicsFamily;
        std::optional<std::uint32_t> mComputeFamily;
        std::optional<std::uint32_t> mTransferFamily;
        std::optional<std::uint32_t> mPresentFamily;

        std::vector<vk::DeviceQueueCreateInfo> mQueueCreateInfos;
    };
}