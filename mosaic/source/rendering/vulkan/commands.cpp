#include "../../../include/rendering/vulkan/commands.hpp"
#include "../../../include/application/application.hpp"

void Mosaic::CreateCommandResources(
    ApplicationData* applicationData,
    vk::UniqueDevice& device,
    vk::UniqueCommandPool& commandPool,
    std::vector<vk::UniqueCommandBuffer>& commandBuffers,
    std::uint32_t swapchainSize)
{
    vk::CommandPoolCreateInfo poolCreateInfo = {};
    poolCreateInfo.flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer;

    try
    {
        commandPool = device->createCommandPoolUnique(poolCreateInfo);

        commandBuffers.resize(swapchainSize);

        vk::CommandBufferAllocateInfo allocateInfo = {};
        allocateInfo.commandPool = *commandPool;
        allocateInfo.level = vk::CommandBufferLevel::ePrimary;
        allocateInfo.commandBufferCount = static_cast<uint32_t>(swapchainSize);

        commandBuffers = device->allocateCommandBuffersUnique(allocateInfo);
    }
    catch (const vk::SystemError& error)
    {
        applicationData->Console.LogError("Failed to create command buffers: {}", error.what());
    }
}