#pragma once

#include "../../application/renderer.hpp"

#include "commands.hpp"
#include "devices.hpp"
#include "instance.hpp"
#include "queues.hpp"
#include "surface.hpp"
#include "swapchain.hpp"

namespace Mosaic
{
    class VulkanRenderer : public RendererInterface
    {
    public:
        VulkanRenderer(ApplicationData* applicationData);

    private:
        void Create() override;
        void CreateSwapchain();

        void Update() override;

        void LoadConfig() override;

        VulkanInstance mInstance;
        VulkanPhysicalDevice mPhysicalDevice;
        VulkanDevice mDevice;
        VulkanQueues mQueues;
        VulkanSurface mSurface;
        VulkanSwapchain mSwapchain;
        VulkanRenderPass mRenderPass;
        VulkanCommandSystem mCommandSystem;

        std::vector<VulkanFramebuffer> mFramebuffers;

        ApplicationData* mApplicationData;

        bool mRebuildSwapchainSuboptimal;
        bool mRebuildSwapchainOutOfDate;

        friend class VulkanSwapchain;
        friend class VulkanDevice;
    };
}