#include "../../../include/rendering/vulkan/commands.hpp"
#include "../../../include/rendering/vulkan/devices.hpp"
#include "../../../include/rendering/vulkan/queues.hpp"
#include "../../../include/rendering/vulkan/swapchain.hpp"

#include "../../../include/application/console.hpp"

void Mosaic::VulkanCommandSystem::Create(VulkanDevice& device, VulkanQueues& queues)
{
    vk::CommandPoolCreateInfo poolCreateInfo{
        vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
        queues.GetGraphicsQueueFamily()};

    auto result = device.Get().createCommandPoolUnique(poolCreateInfo);

    if (result.result != vk::Result::eSuccess)
    {
        Console::Throw("Error creating command pool: {}", vk::to_string(result.result));
    }

    mCommandPool = std::move(result.value);
}

void Mosaic::VulkanCommandSystem::AllocateCommandBuffers(VulkanDevice& device, VulkanSwapchain& swapchain)
{
    vk::CommandBufferAllocateInfo allocInfo{
        mCommandPool.get(),
        vk::CommandBufferLevel::ePrimary,
        swapchain.GetInFlightFrames()};

    auto result = device.Get().allocateCommandBuffersUnique(allocInfo);

    if (result.result != vk::Result::eSuccess)
    {
        Console::Throw("Error allocating command buffers: {}", vk::to_string(result.result));
    }

    mCommandBuffers = std::move(result.value);
}

void Mosaic::VulkanCommandSystem::BeginFrame(std::uint32_t imageIndex)
{
    auto& commandBuffer = mCommandBuffers[imageIndex];

    vk::CommandBufferBeginInfo beginInfo{
        vk::CommandBufferUsageFlagBits::eSimultaneousUse,
        nullptr};

    auto result = commandBuffer->begin(beginInfo);

    if (result != vk::Result::eSuccess)
    {
        Console::Throw("Error beginning command buffer recording: {}", vk::to_string(result));
    }
}

void Mosaic::VulkanCommandSystem::EndFrame(std::uint32_t imageIndex)
{
    auto& commandBuffer = mCommandBuffers[imageIndex];

    auto result = commandBuffer->end();
    if (result != vk::Result::eSuccess)
    {
        Console::Throw("Error ending command buffer recording: {}", vk::to_string(result));
    }
}

void Mosaic::VulkanCommandSystem::RecordCommands(VulkanRenderPass& renderPass, VulkanFramebuffer& framebuffer, VulkanSwapchain& swapchain, std::uint32_t imageIndex)
{
    auto& commandBuffer = mCommandBuffers[imageIndex];

    vk::RenderPassBeginInfo renderPassBeginInfo{
        renderPass.GetRenderPass(),
        framebuffer.GetFramebuffer(),
        vk::Rect2D({0, 0}, swapchain.GetExtent()),
        0, nullptr};

    commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

    commandBuffer->endRenderPass();
}

vk::CommandBuffer& Mosaic::VulkanCommandSystem::GetCommandBuffer(std::uint32_t imageIndex)
{
    return *mCommandBuffers[imageIndex];
}