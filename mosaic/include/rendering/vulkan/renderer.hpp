#pragma once

#include "../../application/renderer.hpp"

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_enums.hpp>

namespace Mosaic
{
    class VulkanRenderer : public RendererInterface
    {
    public:
        VulkanRenderer(ApplicationData* applicationData);

    private:
        void Create() override;

        void PreUpdate() override;
        void PostUpdate() override;

        void LoadConfig() override;

        void CreateResources();

        void Record();
        void Submit();
        void Present();

        void AwaitFrame();
        void AdvanceFrame();

        std::optional<std::uint32_t> AcquireFrame();

        std::uint32_t mImageCount;
        std::uint32_t mImageIndex;
        std::uint32_t mFrameIndex;
        std::uint32_t mGraphicsQueueFamilyIndex;

        ApplicationData* mApplicationData;

        std::vector<vk::Image> mSwapchainImages;
        std::vector<vk::UniqueImageView> mSwapchainImageViews;
        std::vector<vk::UniqueFramebuffer> mSwapchainFramebuffers;
        std::vector<vk::UniqueCommandBuffer> mCommandBuffers;
        std::vector<vk::UniqueSemaphore> mImageAvailableSemaphores;
        std::vector<vk::UniqueSemaphore> mRenderFinishedSemaphores;
        std::vector<vk::UniqueFence> mInFlightFences;

        vk::UniqueInstance mInstance;
        vk::UniqueDevice mDevice;
        vk::PhysicalDevice mPhysicalDevice;
        vk::Queue mGraphicsQueue;
        vk::UniqueSurfaceKHR mSurface;
        vk::UniqueSwapchainKHR mSwapchain;
        vk::UniqueRenderPass mRenderPass;
        vk::SurfaceFormatKHR mSurfaceFormat;
        vk::Extent2D mSwapchainExtent;
        vk::UniqueCommandPool mCommandPool;

        vk::PresentModeKHR mPresentMode;
    };
}