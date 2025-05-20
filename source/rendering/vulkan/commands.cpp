#include "rendering/vulkan/commands.hpp"
#include "rendering/vulkan/devices.hpp"
#include "rendering/vulkan/queues.hpp"
#include "rendering/vulkan/swapchain.hpp"

namespace Mosaic::Internal::Rendering
{
    void VulkanCommandSystem::Create(VulkanDevice& device, VulkanQueues& queues)
    {
        vk::CommandPoolCreateInfo poolCreateInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queues.GetGraphicsQueueFamily()};

        mCommandPool = device.Get().createCommandPoolUnique(poolCreateInfo);
    }

    void VulkanCommandSystem::AllocateCommandBuffers(VulkanDevice& device, VulkanSwapchain& swapchain)
    {
        vk::CommandBufferAllocateInfo allocInfo{mCommandPool.get(), vk::CommandBufferLevel::ePrimary, swapchain.GetImageCount()};

        mCommandBuffers = device.Get().allocateCommandBuffersUnique(allocInfo);
    }

    void VulkanCommandSystem::BeginFrame(Types::UI32 imageIndex)
    {
        auto& commandBuffer = mCommandBuffers[imageIndex];

        vk::CommandBufferBeginInfo beginInfo{vk::CommandBufferUsageFlagBits::eSimultaneousUse, nullptr};

        commandBuffer->begin(beginInfo);
    }

    void VulkanCommandSystem::EndFrame(Types::UI32 imageIndex)
    {
        auto& commandBuffer = mCommandBuffers[imageIndex];

        commandBuffer->end();
    }

    void VulkanCommandSystem::RecordCommands(VulkanRenderPass& renderPass, VulkanFramebuffer& framebuffer, VulkanSwapchain& swapchain, Types::UI32 imageIndex, const Types::Vector4<Types::F32>& clear)
    {
        auto& commandBuffer = mCommandBuffers[imageIndex];

        vk::ClearValue clearColour = vk::ClearColorValue(std::array{clear.X, clear.Y, clear.Z, clear.W});

        vk::RenderPassBeginInfo renderPassBeginInfo{renderPass.GetRenderPass(), framebuffer.GetFramebuffer(), vk::Rect2D({0, 0}, swapchain.GetExtent()), 1, &clearColour};

        commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
        commandBuffer->endRenderPass();
    }

    vk::CommandBuffer& VulkanCommandSystem::GetCommandBuffer(Types::UI32 imageIndex)
    {
        return *mCommandBuffers[imageIndex];
    }

    void VulkanCommandSystem::SubmitFrame(vk::Queue graphicsQueue, const VulkanFrameSubmitDescriptor& info)
    {
        vk::SubmitInfo submitInfo{1, &info.WaitSemaphore, &info.WaitStage, 1, &info.Command, 1, &info.SignalSemaphore};

        graphicsQueue.submit(submitInfo, info.InFlightFence);
    }

    void VulkanCommandSystem::Reset()
    {
        if (mCommandPool)
        {
            mCommandBuffers.clear();
            mCommandPool.reset();
        }
    }
}