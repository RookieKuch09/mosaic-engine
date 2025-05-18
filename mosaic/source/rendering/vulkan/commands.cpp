#include "../../../include/rendering/vulkan/commands.hpp"
#include "../../../include/rendering/vulkan/devices.hpp"
#include "../../../include/rendering/vulkan/queues.hpp"
#include "../../../include/rendering/vulkan/swapchain.hpp"

void Mosaic::VulkanCommandSystem::Create(VulkanDevice& device, VulkanQueues& queues)
{
    vk::CommandPoolCreateInfo poolCreateInfo{vk::CommandPoolCreateFlagBits::eResetCommandBuffer, queues.GetGraphicsQueueFamily()};

    mCommandPool = device.Get().createCommandPoolUnique(poolCreateInfo);
}

void Mosaic::VulkanCommandSystem::AllocateCommandBuffers(VulkanDevice& device, VulkanSwapchain& swapchain)
{
    vk::CommandBufferAllocateInfo allocInfo{mCommandPool.get(), vk::CommandBufferLevel::ePrimary, swapchain.GetImageCount()};

    mCommandBuffers = device.Get().allocateCommandBuffersUnique(allocInfo);
}

void Mosaic::VulkanCommandSystem::BeginFrame(std::uint32_t imageIndex)
{
    auto& commandBuffer = mCommandBuffers[imageIndex];

    vk::CommandBufferBeginInfo beginInfo{vk::CommandBufferUsageFlagBits::eSimultaneousUse, nullptr};

    commandBuffer->begin(beginInfo);
}

void Mosaic::VulkanCommandSystem::EndFrame(std::uint32_t imageIndex)
{
    auto& commandBuffer = mCommandBuffers[imageIndex];

    commandBuffer->end();
}

void Mosaic::VulkanCommandSystem::RecordCommands(VulkanRenderPass& renderPass, VulkanFramebuffer& framebuffer, VulkanSwapchain& swapchain, std::uint32_t imageIndex, const std::array<float, 4>& clear)
{
    auto& commandBuffer = mCommandBuffers[imageIndex];

    vk::ClearValue clearColour = vk::ClearColorValue(clear);

    vk::RenderPassBeginInfo renderPassBeginInfo{renderPass.GetRenderPass(), framebuffer.GetFramebuffer(), vk::Rect2D({0, 0}, swapchain.GetExtent()), 1, &clearColour};

    commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
    commandBuffer->endRenderPass();
}

vk::CommandBuffer& Mosaic::VulkanCommandSystem::GetCommandBuffer(std::uint32_t imageIndex)
{
    return *mCommandBuffers[imageIndex];
}

void Mosaic::VulkanCommandSystem::SubmitFrame(vk::Queue graphicsQueue, const VulkanFrameSubmitDescriptor& info)
{
    vk::SubmitInfo submitInfo{1, &info.WaitSemaphore, &info.WaitStage, 1, &info.Command, 1, &info.SignalSemaphore};

    graphicsQueue.submit(submitInfo, info.InFlightFence);
}

void Mosaic::VulkanCommandSystem::Reset()
{
    if (mCommandPool)
    {
        mCommandBuffers.clear();
        mCommandPool.reset();
    }
}