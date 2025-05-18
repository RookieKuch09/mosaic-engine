#pragma once

#include <vulkan/vulkan.hpp>

namespace Mosaic
{
    class VulkanInstance;
    class VulkanPhysicalDevice;
    class Window;

    class VulkanSurface
    {
    public:
        void SelectFormat(VulkanPhysicalDevice& physicalDevice, vk::Format format, vk::ColorSpaceKHR space);

        void Create(Window& window, VulkanInstance& instance);

        vk::SurfaceKHR& GetHandle();
        vk::SurfaceFormatKHR& GetFormat();

    private:
        vk::UniqueSurfaceKHR mSurface;
        vk::SurfaceFormatKHR mFormat;
    };
}