#pragma once

#include "rendering/renderer.hpp"

#include "rendering/vulkan/commands.hpp"
#include "rendering/vulkan/devices.hpp"
#include "rendering/vulkan/instance.hpp"
#include "rendering/vulkan/queues.hpp"
#include "rendering/vulkan/surface.hpp"
#include "rendering/vulkan/swapchain.hpp"

#include "utilities/numerics.hpp"
#include "utilities/vector.hpp"

namespace Mosaic::Internal::Windowing
{
    struct WindowResizeEvent;
}

namespace Mosaic::Internal::Rendering
{
    class VulkanRenderer : public RendererInterface
    {
    public:
        VulkanRenderer(Renderer& renderer);

    private:
        void Create() override;
        void Update() override;
        void LoadConfig() override;

        void CreateSwapchain();

        void ResizeCallback(const Windowing::WindowResizeEvent& event);

        Types::Vec2<Types::UI32> mWindowSize;
        VulkanInstance mInstance;
        VulkanPhysicalDevice mPhysicalDevice;
        VulkanDevice mDevice;
        VulkanQueues mQueues;
        VulkanSurface mSurface;
        VulkanSwapchain mSwapchain;
        VulkanRenderPass mRenderPass;
        VulkanCommandSystem mCommandSystem;
        Renderer& mRenderer;

        std::vector<VulkanFramebuffer> mFramebuffers;

        bool mRebuildSwapchainSuboptimal;
        bool mRebuildSwapchainOutOfDate;

        friend class VulkanSwapchain;
        friend class VulkanDevice;
    };
}