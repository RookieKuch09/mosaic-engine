#pragma once

#include <vulkan/vulkan.hpp>

#include <glm/glm.hpp>

namespace Mosaic
{
    class Window;

    enum class RendererVSync : int;

    struct ApplicationData;

    class VulkanPhysicalDevice;
    class VulkanDevice;
    class VulkanSurface;
    class VulkanRenderPass;
    class VulkanSwapchain;

    struct VulkanFrameSyncObjects
    {
        vk::UniqueSemaphore ImageAvailable;
        vk::UniqueSemaphore RenderFinished;
        vk::UniqueFence InFlight;
    };

    class VulkanFramebuffer
    {
    public:
        void Create(VulkanDevice& device, VulkanSurface& surface, VulkanRenderPass& renderPass, VulkanSwapchain& swapchain);

        vk::Framebuffer& GetFramebuffer();
        vk::ImageView& GetImageView();

        std::uint32_t& GetIndex();

    private:
        vk::UniqueFramebuffer mFramebuffer;
        vk::UniqueImageView mImageView;

        std::uint32_t mIndex;
    };

    class VulkanRenderPass
    {
    public:
        void Create(VulkanDevice& device, VulkanSurface& surface);

        vk::RenderPass& GetRenderPass();

    private:
        vk::UniqueRenderPass mRenderPass;
    };

    class VulkanSwapchain
    {
    public:
        void Create(Window& window, VulkanDevice& device, VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface, RendererVSync vsync);
        void CreateSyncObjects(VulkanDevice& device);

        vk::SwapchainKHR& GetSwapchain();
        vk::Extent2D& GetExtent();
        vk::Image& GetImage(std::uint32_t index);
        std::uint32_t GetImageCount();
        std::uint32_t GetInFlightFrames();

    private:
        vk::UniqueSwapchainKHR mSwapchain;
        vk::Extent2D mSwapchainExtent;

        vk::PresentModeKHR mPresentMode;

        std::uint32_t mImageCount;
        std::uint32_t mFramesInFlight;

        std::vector<vk::Image> mSwapchainImages;
        std::vector<VulkanFrameSyncObjects> mSyncFrames;
        std::vector<vk::Fence> mImagesInFlight;
    };
}