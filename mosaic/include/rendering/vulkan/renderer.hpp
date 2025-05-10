#pragma once

#include "../../application/renderer.hpp"

#include "devices.hpp"
#include "instance.hpp"
#include "queues.hpp"
#include "surface.hpp"
#include "swapchain.hpp"

#include <vulkan/vulkan.hpp>

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

        VulkanInstance mInstance;
        VulkanPhysicalDevice mPhysicalDevice;
        VulkanDevice mDevice;
        VulkanQueues mQueues;
        VulkanSurface mSurface;
        VulkanSwapchain mSwapchain;
        VulkanRenderPass mRenderPass;

        std::vector<VulkanFramebuffer> mFramebuffers;

        ApplicationData* mApplicationData;
    };
}