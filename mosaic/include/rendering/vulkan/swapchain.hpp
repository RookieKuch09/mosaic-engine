#pragma once

#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

namespace Mosaic
{
    struct ApplicationData;

    enum class RendererVSync : int;
}

namespace Mosaic
{
    void CreateSwapchain(
        vk::UniqueDevice& device,
        vk::UniqueSurfaceKHR& surface,
        vk::SurfaceFormatKHR& format,
        vk::Extent2D extent,
        std::uint32_t minBuffers,
        vk::PresentModeKHR presentMode,
        vk::UniqueSwapchainKHR& swapchain);

    void GetSwapchainData(
        vk::PhysicalDevice& physicalDevice,
        vk::UniqueSurfaceKHR& surface,
        vk::SurfaceFormatKHR& format,
        vk::Extent2D& extent,
        RendererVSync preferredMode,
        vk::PresentModeKHR& presentMode,
        const glm::uvec2& size,
        uint32_t& imageCount);

    void CreateRenderPass(
        vk::UniqueDevice& device,
        vk::SurfaceFormatKHR& format,
        vk::UniqueRenderPass& renderPass);

    void CreateSwapchainImages(
        vk::UniqueDevice& device,
        vk::SurfaceFormatKHR& format,
        std::vector<vk::Image>& swapchainImages,
        std::vector<vk::UniqueImageView>& imageViews);

    void CreateFramebuffers(
        vk::UniqueDevice& device,
        vk::SurfaceFormatKHR& format,
        vk::UniqueRenderPass& renderPass,
        vk::Extent2D& swapchainExtent,
        std::vector<vk::UniqueFramebuffer>& swapchainFramebuffers,
        std::vector<vk::UniqueImageView>& swapchainImageViews);
}