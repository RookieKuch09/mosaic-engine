#pragma once

#include <string>
#include <vector>

#include <vulkan/vulkan.hpp>

namespace Mosaic
{
    struct ApplicationData;
}

namespace Mosaic
{
    void CreateInstance(
        vk::UniqueInstance& instance,
        const std::vector<std::string>& requestedLayers,
        const std::vector<std::string>& requestedExtensions);

    void GetLayers(
        std::vector<std::string>& layers,
        const std::vector<std::string>& requested);

    void GetInstanceExtensions(
        std::vector<std::string>& extensions,
        const std::vector<std::string>& requested);
}