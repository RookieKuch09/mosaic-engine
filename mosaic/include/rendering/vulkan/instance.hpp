#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace Mosaic
{
    struct ApplicationData;
}

namespace Mosaic::Backend::Vulkan
{
    void CreateInstance(
        ApplicationData* applicationData,
        vk::UniqueInstance& instance,
        const std::vector<std::string>& requestedLayers,
        const std::vector<std::string>& requestedExtensions);

    void GetLayers(
        ApplicationData* applicationData,
        std::vector<std::string>& layers,
        const std::vector<std::string>& requested);

    void GetInstanceExtensions(
        ApplicationData* applicationData,
        std::vector<std::string>& extensions,
        const std::vector<std::string>& requested);
}