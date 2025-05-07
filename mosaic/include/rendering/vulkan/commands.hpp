#pragma once

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>

namespace Mosaic
{
    struct ApplicationData;
}

namespace Mosaic
{
    void CreateCommandResources(
        ApplicationData* applicationData,
        vk::UniqueDevice& device,
        vk::UniqueCommandPool& commandPool,
        std::vector<vk::UniqueCommandBuffer>& commandBuffers,
        std::uint32_t swapchainSize);
}
