#pragma once

#include "utilities/numerics.hpp"
#include "utilities/vector.hpp"

#include <vulkan/vulkan.hpp>

namespace Mosaic::Internal::Rendering
{
    enum class RendererVSync : int;

    struct ApplicationData;

    class VulkanPhysicalDevice;
    class VulkanDevice;
    class VulkanSurface;
    class VulkanRenderPass;
    class VulkanSwapchain;
    class VulkanQueues;
    class VulkanRenderer;

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

        void Reset();

        vk::Framebuffer& GetFramebuffer();
        vk::ImageView& GetImageView();

        Types::UI32& GetIndex();

    private:
        vk::UniqueFramebuffer mFramebuffer;
        vk::UniqueImageView mImageView;

        Types::UI32 mIndex;
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
        void Create(const Types::Vector2<Types::UI32>& size, VulkanDevice& device, VulkanPhysicalDevice& physicalDevice, VulkanSurface& surface, RendererVSync vsync);
        void CreateSyncObjects(VulkanDevice& device);

        void Reset();

        vk::SwapchainKHR& GetSwapchain();
        vk::Extent2D& GetExtent();
        vk::Image& GetImage(Types::UI32 index);
        Types::UI32 GetImageCount();
        Types::UI32 GetInFlightFrames();
        Types::UI32 GetCurrentFrame();
        std::vector<VulkanFrameSyncObjects>& GetSyncFrames();

        void PresentFrame(VulkanRenderer& renderer, VulkanQueues& queues, Types::UI32 imageIndex);

        void IncrementFrame();

    private:
        vk::UniqueSwapchainKHR mSwapchain;
        vk::Extent2D mSwapchainExtent;

        vk::PresentModeKHR mPresentMode;

        Types::UI32 mImageCount;
        Types::UI32 mFramesInFlight;
        Types::UI32 mCurrentFrame;

        std::vector<vk::Image> mSwapchainImages;
        std::vector<VulkanFrameSyncObjects> mSyncFrames;
        std::vector<vk::Fence> mImagesInFlight;
    };
}