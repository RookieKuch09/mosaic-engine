#pragma once

#include "utilities/numerics.hpp"
#include "utilities/vector.hpp"

#include <vulkan/vulkan.hpp>

namespace Mosaic::Internal::Rendering
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
        void BeginFrame(Types::UI32 imageIndex);
        void EndFrame(Types::UI32 imageIndex);
        void RecordCommands(VulkanRenderPass& renderPass, VulkanFramebuffer& framebuffer, VulkanSwapchain& swapchain, Types::UI32 imageIndex, const Types::Vector4<Types::F32>& clear);

        void Reset();

        void SubmitFrame(vk::Queue graphicsQueue, const VulkanFrameSubmitDescriptor& info);

        vk::CommandBuffer& GetCommandBuffer(Types::UI32 imageIndex);

    private:
        vk::UniqueCommandPool mCommandPool;
        std::vector<vk::UniqueCommandBuffer> mCommandBuffers;
    };
}
