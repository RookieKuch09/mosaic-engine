#include "../../../include/rendering/vulkan/surface.hpp"
#include "../../../include/rendering/vulkan/devices.hpp"
#include "../../../include/rendering/vulkan/instance.hpp"

#include "../../../include/application/console.hpp"
#include "../../../include/application/window.hpp"

void Mosaic::VulkanSurface::SelectFormat(VulkanPhysicalDevice& physicalDevice, vk::Format format, vk::ColorSpaceKHR space)
{
    auto formats = physicalDevice.Get().getSurfaceFormatsKHR(*mSurface);

    if (formats.empty())
    {
        Console::Throw("vk::Surface does not support any formats");
    }

    auto match = [&](auto const& f)
    {
        return f.format == format and f.colorSpace == space;
    };

    auto itf = std::find_if(formats.begin(), formats.end(), match);

    mFormat = (itf != formats.end() ? *itf : formats[0]);
}

void Mosaic::VulkanSurface::Create(Window& window, VulkanInstance& instance)
{
    window.GetVulkanWindowSurface(*mSurface, instance.Get());
}

vk::SurfaceKHR& Mosaic::VulkanSurface::GetHandle()
{
    return *mSurface;
}

vk::SurfaceFormatKHR& Mosaic::VulkanSurface::GetFormat()
{
    return mFormat;
}