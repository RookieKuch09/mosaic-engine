#pragma once

#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

namespace Mosaic
{
    class VulkanDevice;
    class VulkanQueues;
    class VulkanSwapchain;
    class VulkanRenderPass;
    class VulkanFramebuffer;

    struct VulkanFrameSubmitDescriptor
    {
        vk::CommandBuffer Command;
        vk::Semaphore WaitSemaphore;
        vk::Semaphore SignalSemaphore;
        vk::Fence InFlightFence;
        vk::PipelineStageFlags WaitStage;
    };

    class VulkanCommandSystem
    {
    public:
        void Create(VulkanDevice& device, VulkanQueues& queues);
        void AllocateCommandBuffers(VulkanDevice& device, VulkanSwapchain& swapchain);
        void BeginFrame(std::uint32_t imageIndex);
        void EndFrame(std::uint32_t imageIndex);
        void RecordCommands(VulkanRenderPass& renderPass, VulkanFramebuffer& framebuffer, VulkanSwapchain& swapchain, std::uint32_t imageIndex, const std::array<float, 4>& clear);

        void Reset();

        void SubmitFrame(vk::Queue graphicsQueue, const VulkanFrameSubmitDescriptor& info);

        vk::CommandBuffer& GetCommandBuffer(std::uint32_t imageIndex);

    private:
        vk::UniqueCommandPool mCommandPool;
        std::vector<vk::UniqueCommandBuffer> mCommandBuffers;
    };
}
