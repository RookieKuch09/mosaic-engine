#include "../../../include/rendering/vulkan/commands.hpp"

void Mosaic::CreateCommandResources(
    vk::UniqueDevice& device,
    vk::UniqueCommandPool& commandPool,
    std::vector<vk::UniqueCommandBuffer>& commandBuffers,
    std::uint32_t swapchainSize)
{
    vk::CommandPoolCreateInfo poolCreateInfo = {};
    poolCreateInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

    commandPool = device->createCommandPoolUnique(poolCreateInfo);

    commandBuffers.resize(swapchainSize);

    vk::CommandBufferAllocateInfo allocateInfo = {};
    allocateInfo.commandPool = *commandPool;
    allocateInfo.level = vk::CommandBufferLevel::ePrimary;
    allocateInfo.commandBufferCount = static_cast<uint32_t>(swapchainSize);

    commandBuffers = device->allocateCommandBuffersUnique(allocateInfo);
}