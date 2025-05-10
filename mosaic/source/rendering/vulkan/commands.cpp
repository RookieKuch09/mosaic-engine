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
        swapchain.GetImageCount()};

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

void Mosaic::VulkanCommandSystem::RecordCommands(VulkanRenderPass& renderPass, VulkanFramebuffer& framebuffer, VulkanSwapchain& swapchain, std::uint32_t imageIndex, const glm::fvec4& clear)
{
    auto& commandBuffer = mCommandBuffers[imageIndex];

    vk::ClearValue clearColour = vk::ClearColorValue(std::array<float, 4>{clear.r, clear.g, clear.b, clear.a});

    vk::RenderPassBeginInfo renderPassBeginInfo{
        renderPass.GetRenderPass(),
        framebuffer.GetFramebuffer(),
        vk::Rect2D({0, 0}, swapchain.GetExtent()),
        1, &clearColour};

    commandBuffer->beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);

    commandBuffer->endRenderPass();
}

vk::CommandBuffer& Mosaic::VulkanCommandSystem::GetCommandBuffer(std::uint32_t imageIndex)
{
    return *mCommandBuffers[imageIndex];
}

void Mosaic::VulkanCommandSystem::SubmitFrame(vk::Queue graphicsQueue, const VulkanFrameSubmitDescriptor& info)
{
    vk::SubmitInfo submitInfo{
        1, &info.WaitSemaphore,
        &info.WaitStage,
        1, &info.Command,
        1, &info.SignalSemaphore};

    auto result = graphicsQueue.submit(submitInfo, info.InFlightFence);

    if (result != vk::Result::eSuccess)
    {
        Console::Throw("Failed to submit frame: {}", vk::to_string(result));
    }
}

void Mosaic::VulkanCommandSystem::Reset()
{
    if (mCommandPool)
    {
        mCommandBuffers.clear();
        mCommandPool.reset();
    }
}