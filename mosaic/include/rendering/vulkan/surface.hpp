#pragma once

#include <vulkan/vulkan.hpp>

namespace Mosaic
{
    class VulkanInstance;
    class Window;

    class VulkanSurface
    {
    public:
        void Create(Window& window, VulkanInstance& instance);

        vk::SurfaceKHR& Get();

    private:
        vk::UniqueSurfaceKHR mSurface;
    };
}