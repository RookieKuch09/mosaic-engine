#include "../../../include/rendering/vulkan/instance.hpp"
#include "../../../include/application/application.hpp"

void Mosaic::Backend::Vulkan::CreateInstance(ApplicationData* applicationData, vk::UniqueInstance& instance, const std::vector<std::string>& layers, const std::vector<std::string>& extensions)
{
    auto getVectorCStrings = [](const std::vector<std::string>& vector)
    {
        std::vector<const char*> result;

        for (const auto& element : vector)
        {
            result.push_back(element.c_str());
        }

        return result;
    };

    vk::ApplicationInfo appInfo = {
        "FILLER_NAME",
        VK_MAKE_VERSION(0, 0, 0),
        "Mosaic",
        VK_MAKE_VERSION(0, 0, 0),
        VK_API_VERSION_1_4,
    };

    auto clayers = getVectorCStrings(layers);
    auto cextensions = getVectorCStrings(extensions);

    vk::InstanceCreateInfo instanceInfo = {
        {},
        &appInfo,
        static_cast<unsigned int>(clayers.size()),
        clayers.data(),
        static_cast<unsigned int>(cextensions.size()),
        cextensions.data()};

    try
    {
        instance = vk::createInstanceUnique(instanceInfo);
    }
    catch (const vk::SystemError& error)
    {
        applicationData->Console.LogError("Failed to create Vulkan instance: {}", error.what());
    }

    if (not instance)
    {
        applicationData->Console.LogError("Failed to create Vulkan instance, unknown error");
    }
}

void Mosaic::Backend::Vulkan::GetLayers(ApplicationData* applicationData, std::vector<std::string>& layers, const std::vector<std::string>& requested)
{
    std::vector<vk::LayerProperties> availableLayers = vk::enumerateInstanceLayerProperties();

    for (const auto& request : requested)
    {
        auto match = [&](const vk::LayerProperties& lp)
        {
            return request == lp.layerName;
        };

        auto found = std::find_if(availableLayers.begin(), availableLayers.end(), match);

        if (found != availableLayers.end())
        {
            layers.push_back(request);
        }
        else
        {
            applicationData->Console.LogWarning("Requested Vulkan layer is not available: {}", request);
        }
    }
}

void Mosaic::Backend::Vulkan::GetInstanceExtensions(ApplicationData* applicationData, std::vector<std::string>& extensions, const std::vector<std::string>& requested)
{
    std::vector<vk::ExtensionProperties> availableExtensions = vk::enumerateInstanceExtensionProperties();

    for (const auto& request : requested)
    {
        auto match = [&](const vk::ExtensionProperties& ep)
        {
            return request == ep.extensionName;
        };

        auto found = std::find_if(availableExtensions.begin(), availableExtensions.end(), match);

        if (found != availableExtensions.end())
        {
            extensions.push_back(request);
        }
        else
        {
            applicationData->Console.LogWarning("Requested Vulkan base extension is not available: {}", request);
        }
    }
}
