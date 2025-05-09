#include "../../../include/rendering/vulkan/queues.hpp"
#include "../../../include/rendering/vulkan/devices.hpp"

#include <set>

void Mosaic::VulkanQueues::Discover(VulkanPhysicalDevice& physicalDevice)
{
    auto& gpu = physicalDevice.Get();

    const auto families = gpu.getQueueFamilyProperties();

    for (std::uint32_t index = 0; index < families.size(); index++)
    {
        const auto& props = families[index];

        if (not mGraphicsFamily and props.queueFlags bitand vk::QueueFlagBits::eGraphics)
        {
            mGraphicsFamily = index;
        }

        if (not mComputeFamily and props.queueFlags bitand vk::QueueFlagBits::eCompute)
        {
            if (not(props.queueFlags bitand vk::QueueFlagBits::eGraphics))
            {
                mComputeFamily = index;
            }
        }

        if (not mTransferFamily and props.queueFlags bitand vk::QueueFlagBits::eTransfer)
        {
            if (not(props.queueFlags bitand vk::QueueFlagBits::eGraphics) and not(props.queueFlags bitand vk::QueueFlagBits::eCompute))
            {
                mTransferFamily = index;
            }
        }
    }

    ResolveFallbacks();

    std::set<std::uint32_t> uniqueFamilies{
        mGraphicsFamily.value(),
        mComputeFamily.value(),
        mTransferFamily.value()};

    float priority = 1.0f;

    for (std::uint32_t family : uniqueFamilies)
    {
        vk::DeviceQueueCreateInfo info{};
        info.queueFamilyIndex = family;
        info.queueCount = 1;
        info.pQueuePriorities = &priority;

        mQueueCreateInfos.push_back(info);
    }
}

void Mosaic::VulkanQueues::ResolveFallbacks()
{
    if (not mComputeFamily)
    {
        mComputeFamily = mGraphicsFamily;
    }

    if (not mTransferFamily)
    {
        mTransferFamily = mComputeFamily;
    }
}

void Mosaic::VulkanQueues::Load(VulkanDevice& device)
{
    auto& logicalDevice = device.Get();

    mGraphicsQueue = logicalDevice.getQueue(mGraphicsFamily.value(), 0);
    mComputeQueue = logicalDevice.getQueue(mComputeFamily.value(), 0);
    mTransferQueue = logicalDevice.getQueue(mTransferFamily.value(), 0);
}

std::vector<vk::DeviceQueueCreateInfo>& Mosaic::VulkanQueues::GetQueueCreateInfo()
{
    return mQueueCreateInfos;
}