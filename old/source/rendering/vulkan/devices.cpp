#include "rendering/vulkan/devices.hpp"
#include "rendering/vulkan/instance.hpp"
#include "rendering/vulkan/queues.hpp"
#include "rendering/vulkan/renderer.hpp"
#include "rendering/vulkan/swapchain.hpp"

#include "application/console.hpp"

namespace Mosaic::Internal::Rendering
{
    void VulkanPhysicalDevice::Select(VulkanInstance& instance)
    {
        auto physicalDevices = instance.Get().enumeratePhysicalDevices();

        if (physicalDevices.empty())
        {
            Console::Throw("No Vulkan-compatible GPU located");
        }

        for (const auto& device : physicalDevices)
        {
            vk::PhysicalDeviceProperties properties = device.getProperties();

            bool discrete = properties.deviceType == vk::PhysicalDeviceType::eDiscreteGpu;
            bool integrated = properties.deviceType == vk::PhysicalDeviceType::eIntegratedGpu;

            if (discrete or integrated)
            {
                mPhysicalDevice = device;

                break;
            }
        }

        if (not mPhysicalDevice)
        {
            Console::Throw("No available GPUs match requriements");
        }
    }

    vk::PhysicalDevice& VulkanPhysicalDevice::Get()
    {
        return mPhysicalDevice;
    }

    void VulkanDevice::GetExtensions(VulkanPhysicalDevice& physicalDevice, const std::unordered_set<std::string>& whitelist, const std::unordered_set<std::string>& blacklist)
    {
        auto extensionProperties = physicalDevice.Get().enumerateDeviceExtensionProperties();

        std::unordered_set<std::string> supportedExtensions;

        for (const auto& extension : extensionProperties)
        {
            supportedExtensions.insert(extension.extensionName);
        }

        mExtensions.clear();

        for (const auto& extension : supportedExtensions)
        {
            if (not blacklist.contains(extension))
            {
                mExtensions.push_back(extension);
            }
        }

        for (const auto& extension : whitelist)
        {
            if (blacklist.contains(extension))
            {
                Console::Throw("Whitelisted extension '{}' is also blacklisted — configuration error", extension);
            }

            if (not supportedExtensions.contains(extension))
            {
                Console::Throw("Requested whitelisted extension '{}' is not supported by the physical device", extension);
            }

            if (std::find(mExtensions.begin(), mExtensions.end(), extension) == mExtensions.end())
            {
                mExtensions.push_back(extension);
            }
        }
    }

    void VulkanDevice::Create(VulkanQueues& queues, VulkanPhysicalDevice& physicalDevice)
    {
        auto getVectorCStrings = [](const std::vector<std::string>& vector)
        {
            std::vector<const char*> result;

            for (const auto& element : vector)
            {
                result.push_back(element.c_str());
            }

            return result;
        };

        auto extensions = getVectorCStrings(mExtensions);

        vk::DeviceCreateInfo deviceCreateInfo = {{}, static_cast<Types::UI32>(queues.GetQueueCreateInfo().size()), queues.GetQueueCreateInfo().data(), 0, nullptr, static_cast<Types::UI32>(extensions.size()), extensions.data()};

        vk::PhysicalDeviceFeatures2 enabledFeatures{};
        vk::PhysicalDeviceVulkan11Features enabled11{};
        vk::PhysicalDeviceVulkan12Features enabled12{};
        vk::PhysicalDeviceVulkan13Features enabled13{};

        enabledFeatures.pNext = &enabled11;
        enabled11.pNext = &enabled12;
        enabled12.pNext = &enabled13;

        physicalDevice.Get().getFeatures2(&enabledFeatures);

        deviceCreateInfo.pNext = &enabledFeatures;

        mDevice = physicalDevice.Get().createDeviceUnique(deviceCreateInfo);
    }

    vk::Device& VulkanDevice::Get()
    {
        return *mDevice;
    }

    void VulkanDevice::AwaitFences(VulkanSwapchain& swapchain)
    {
        vk::Result result;

        result = mDevice->waitForFences(swapchain.GetSyncFrames()[swapchain.GetCurrentFrame()].InFlight.get(), VK_TRUE, UINT64_MAX);

        if (result != vk::Result::eSuccess)
        {
            Console::Throw("Failed to await fences: {}", vk::to_string(result));
        }

        mDevice->resetFences(swapchain.GetSyncFrames()[swapchain.GetCurrentFrame()].InFlight.get());
    }

    Types::UI32 VulkanDevice::GetNextImageIndex(VulkanRenderer& renderer, VulkanSwapchain& swapchain)
    {
        Types::UI32 imageIndex = 0;

        auto semaphore = swapchain.GetSyncFrames()[swapchain.GetCurrentFrame()].ImageAvailable.get();

        try
        {
            auto result = mDevice->acquireNextImageKHR(swapchain.GetSwapchain(), UINT64_MAX, semaphore, nullptr, &imageIndex);

            if (result == vk::Result::eSuboptimalKHR)
            {
                renderer.mRebuildSwapchainSuboptimal = true;
            }
            else if (result == vk::Result::eErrorOutOfDateKHR)
            {
                renderer.mRebuildSwapchainOutOfDate = true;
            }
            else if (result != vk::Result::eSuccess)
            {
                Console::Throw("Failed to acquire next image index: {}", vk::to_string(result));
            }
        }
        catch (const vk::SystemError& error)
        {
            auto result = static_cast<vk::Result>(error.code().value());

            if (result == vk::Result::eSuboptimalKHR)
            {
                renderer.mRebuildSwapchainSuboptimal = true;
            }
            else if (result == vk::Result::eErrorOutOfDateKHR)
            {
                renderer.mRebuildSwapchainOutOfDate = true;
            }
            else if (result != vk::Result::eSuccess)
            {
                Console::Throw("Failed to acquire next image index: {}", vk::to_string(result));
            }
        }

        return imageIndex;
    }

    void VulkanDevice::WaitIdle()
    {
        mDevice->waitIdle();
    }
}