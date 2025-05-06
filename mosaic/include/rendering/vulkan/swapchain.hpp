#pragma once

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace Mosaic
{
    struct ApplicationData;
}

namespace Mosaic::Backend::Vulkan
{
    void CreateSwapchain(
        ApplicationData* applicationData,
        vk::UniqueDevice& device,
        vk::UniqueSurfaceKHR& surface,
        vk::SurfaceFormatKHR& format,
        vk::Extent2D extent,
        vk::PresentModeKHR presentMode,
        vk::UniqueSwapchainKHR& swapchain);

    void GetSwapchainData(
        ApplicationData* applicationData,
        vk::PhysicalDevice& physicalDevice,
        vk::UniqueSurfaceKHR& surface,
        vk::SurfaceFormatKHR& format,
        vk::Extent2D& extent,
        vk::PresentModeKHR preferredMode,
        vk::PresentModeKHR& presentMode,
        uint32_t& imageCount);

    void CreateRenderPass(
        ApplicationData* applicationData,
        vk::UniqueDevice& device,
        vk::SurfaceFormatKHR& format,
        vk::UniqueRenderPass& renderPass);

    void CreateSwapchainImages(
        ApplicationData* applicationData,
        vk::UniqueDevice& device,
        vk::SurfaceFormatKHR& format,
        std::vector<vk::Image>& swapchainImages,
        std::vector<vk::UniqueImageView>& imageViews);

    void CreateFramebuffers(
        ApplicationData* applicationData,
        vk::UniqueDevice& device,
        vk::SurfaceFormatKHR& format,
        vk::UniqueRenderPass& renderPass,
        vk::Extent2D& swapchainExtent,
        std::vector<vk::UniqueFramebuffer>& swapchainFramebuffers,
        std::vector<vk::UniqueImageView>& swapchainImageViews);
}