#pragma once

#include <vulkan/vulkan.hpp>

namespace Mosaic
{
    class VulkanDevice;
    class VulkanQueues;
    class VulkanSwapchain;
    class VulkanRenderPass;
    class VulkanFramebuffer;

    class VulkanCommandSystem
    {
    public:
        void Create(VulkanDevice& device, VulkanQueues& queues);
        void AllocateCommandBuffers(VulkanDevice& device, VulkanSwapchain& swapchain);
        void BeginFrame(std::uint32_t imageIndex);
        void EndFrame(std::uint32_t imageIndex);
        void RecordCommands(VulkanRenderPass& renderPass, VulkanFramebuffer& framebuffer, VulkanSwapchain& swapchain, std::uint32_t imageIndex);

        vk::CommandBuffer& GetCommandBuffer(std::uint32_t imageIndex);

    private:
        vk::UniqueCommandPool mCommandPool;
        std::vector<vk::UniqueCommandBuffer> mCommandBuffers;
    };
}
