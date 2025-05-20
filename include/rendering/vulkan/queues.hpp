#pragma once

#include "utilities/numerics.hpp"

#include <vulkan/vulkan.hpp>

namespace Mosaic::Internal::Rendering
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

        Types::UI32 GetGraphicsQueueFamily() const;
        Types::UI32 GetComputeQueueFamily() const;
        Types::UI32 GetTransferQueueFamily() const;
        Types::UI32 GetPresentQueueFamily() const;

        std::vector<vk::DeviceQueueCreateInfo>& GetQueueCreateInfo();

    private:
        void ResolveFallbacks();

        vk::Queue mGraphicsQueue;
        vk::Queue mComputeQueue;
        vk::Queue mTransferQueue;
        vk::Queue mPresentQueue;

        std::optional<Types::UI32> mGraphicsFamily;
        std::optional<Types::UI32> mComputeFamily;
        std::optional<Types::UI32> mTransferFamily;
        std::optional<Types::UI32> mPresentFamily;

        Types::F32 mQueuePriority;

        std::vector<vk::DeviceQueueCreateInfo> mQueueCreateInfos;
    };
}