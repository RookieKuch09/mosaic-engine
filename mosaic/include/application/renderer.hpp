#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <vulkan/vulkan.hpp>

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

        std::vector<const char*> ExtractVectorStrings(const std::vector<std::string>& vector);

        ApplicationData* mApplicationData;

        std::vector<std::string> mBaseExtensions;
        std::vector<std::string> mDeviceExtensions;
        std::vector<std::string> mLayers;

        vk::UniqueInstance mInstance;
        vk::UniqueDevice mDevice;
        vk::PhysicalDevice mPhysicalDevice;
        vk::Queue mGraphicsQueue;
        vk::UniqueSurfaceKHR mSurface;

        glm::fvec4 mSwapColour;

        friend class Application;
    };
}