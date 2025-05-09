#include "../../../include/rendering/vulkan/surface.hpp"
#include "../../../include/rendering/vulkan/instance.hpp"

#include "../../../include/application/window.hpp"

void Mosaic::VulkanSurface::Create(Window& window, VulkanInstance& instance)
{
    window.GetVulkanWindowSurface(*mSurface, instance.Get());
}

vk::SurfaceKHR& Mosaic::VulkanSurface::Get()
{
    return *mSurface;
}