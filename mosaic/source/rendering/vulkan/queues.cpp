#include "../../../include/rendering/vulkan/queues.hpp"
#include "../../../include/rendering/vulkan/devices.hpp"
#include "../../../include/rendering/vulkan/surface.hpp"

#include <set>

void Mosaic::VulkanQueues::Discover(VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface)
{
    auto& gpu = physicalDevice.Get();

    const auto families = gpu.getQueueFamilyProperties();

    for (std::uint32_t index = 0; index < families.size(); index++)
    {
        const auto& props = families[index];

        if (not mGraphicsFamily and props.queueFlags & vk::QueueFlagBits::eGraphics)
        {
            mGraphicsFamily = index;
        }

        if (not mComputeFamily and props.queueFlags & vk::QueueFlagBits::eCompute)
        {
            if (not(props.queueFlags & vk::QueueFlagBits::eGraphics))
            {
                mComputeFamily = index;
            }
        }

        if (not mTransferFamily and props.queueFlags & vk::QueueFlagBits::eTransfer)
        {
            if (not(props.queueFlags & vk::QueueFlagBits::eGraphics) and not(props.queueFlags & vk::QueueFlagBits::eCompute))
            {
                mTransferFamily = index;
            }
        }

        bool alreadyUsed = (mGraphicsFamily and mGraphicsFamily.value() == index) or (mComputeFamily and mComputeFamily.value() == index) or (mTransferFamily and mTransferFamily.value() == index);

        auto support = gpu.getSurfaceSupportKHR(index, surface.GetHandle());

        if (not alreadyUsed and not mPresentFamily and support)
        {
            mPresentFamily = index;
        }
    }

    ResolveFallbacks();

    std::set<std::uint32_t> uniqueFamilies = {mGraphicsFamily.value(), mComputeFamily.value(), mTransferFamily.value(), mPresentFamily.value()};

    mQueuePriority = 1.0;

    for (std::uint32_t family : uniqueFamilies)
    {
        vk::DeviceQueueCreateInfo info{};
        info.queueFamilyIndex = family;
        info.queueCount = 1;
        info.pQueuePriorities = &mQueuePriority;

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

    if (not mPresentFamily)
    {
        mPresentFamily = mGraphicsFamily;
    }
}

void Mosaic::VulkanQueues::Load(VulkanDevice& device)
{
    auto& logicalDevice = device.Get();

    mGraphicsQueue = logicalDevice.getQueue(mGraphicsFamily.value(), 0);
    mComputeQueue = logicalDevice.getQueue(mComputeFamily.value(), 0);
    mTransferQueue = logicalDevice.getQueue(mTransferFamily.value(), 0);
    mPresentQueue = logicalDevice.getQueue(mPresentFamily.value(), 0);
}

std::vector<vk::DeviceQueueCreateInfo>& Mosaic::VulkanQueues::GetQueueCreateInfo()
{
    return mQueueCreateInfos;
}

vk::Queue Mosaic::VulkanQueues::GetGraphicsQueue() const
{
    return mGraphicsQueue;
}

vk::Queue Mosaic::VulkanQueues::GetComputeQueue() const
{
    return mComputeQueue;
}

vk::Queue Mosaic::VulkanQueues::GetTransferQueue() const
{
    return mTransferQueue;
}

vk::Queue Mosaic::VulkanQueues::GetPresentQueue() const
{
    return mPresentQueue;
}

std::uint32_t Mosaic::VulkanQueues::GetGraphicsQueueFamily() const
{
    return mGraphicsFamily.value();
}

std::uint32_t Mosaic::VulkanQueues::GetComputeQueueFamily() const
{
    return mComputeFamily.value_or(mGraphicsFamily.value());
}

std::uint32_t Mosaic::VulkanQueues::GetTransferQueueFamily() const
{
    return mTransferFamily.value_or(mComputeFamily.value_or(mGraphicsFamily.value()));
}

std::uint32_t Mosaic::VulkanQueues::GetPresentQueueFamily() const
{
    return mPresentFamily.value_or(mGraphicsFamily.value());
}