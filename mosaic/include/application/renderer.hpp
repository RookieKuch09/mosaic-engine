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
        void Update();

        void GetBaseExtensions();
        void GetDeviceExtensions();

        void CreateInstance();
        void CreatePhysicalDevice();
        void CreateGraphicsQueueAndDevice();
        void CreateSurface();
        void CreateSwapchain();
        void CreateRenderPass();
        void CreateFramebuffers();
        void CreateCommandBuffers();
        void CreateSyncObjects();

        void RecordCommandBuffer(vk::CommandBuffer& commandBuffer, std::uint32_t imageIndex);
        void SubmitCommandBuffer(vk::CommandBuffer& commandBuffer, std::uint32_t imageIndex);

        void PresentImage(std::uint32_t imageIndex);

        void SelectSurfaceFormatAndPresentMode();

        std::vector<const char*> ExtractVectorStrings(const std::vector<std::string>& vector);

        std::uint32_t mImageCount;

        unsigned int mCurrentFrame;

        ApplicationData* mApplicationData;

        std::vector<std::string> mBaseExtensions;
        std::vector<std::string> mDeviceExtensions;
        std::vector<std::string> mLayers;

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