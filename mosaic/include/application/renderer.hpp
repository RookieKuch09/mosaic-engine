#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_handles.hpp>

namespace Mosaic
{
    struct ApplicationData;

    class Renderer
    {
    public:
        Renderer(ApplicationData& applicationData);

        glm::fvec4 GetSwapColour() const;
        void SetSwapColour(const glm::fvec4& colour);

    private:
        void Create();

        void FirstUpdate();
        void SecondUpdate();

        void CreateInstance();
        void CreateDevices();
        void CreateSurface();
        void CreateSwapchain();
        void CreateCommandBuffers();
        void CreateSyncObjects();

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

        glm::fvec4 mSwapColour = glm::fvec4(0.2, 0.3, 0.4, 1.0);

        friend class Application;
    };
}