#include "rendering/vulkan/surface.hpp"
#include "rendering/vulkan/devices.hpp"
#include "rendering/vulkan/instance.hpp"

#include "application/console.hpp"
#include "application/window.hpp"

namespace Mosaic::Internal::Rendering
{
    void VulkanSurface::SelectFormat(VulkanPhysicalDevice& physicalDevice, vk::Format format, vk::ColorSpaceKHR space)
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

    void VulkanSurface::Create(Windowing::Window& window, VulkanInstance& instance)
    {
        window.GetVulkanWindowSurface(*mSurface, instance.Get());
    }

    vk::SurfaceKHR& VulkanSurface::GetHandle()
    {
        return *mSurface;
    }

    vk::SurfaceFormatKHR& VulkanSurface::GetFormat()
    {
        return mFormat;
    }
}