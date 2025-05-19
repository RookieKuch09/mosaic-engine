#include "rendering/vulkan/instance.hpp"

#include "application/console.hpp"
#include "application/window.hpp"

namespace Mosaic::Internal::Rendering
{
    void VulkanInstance::SelectWindowExtensions(const Windowing::Window& window)
    {
        auto extensions = window.GetVulkanRequiredInstanceExtensions();

        auto availableExtensions = vk::enumerateInstanceExtensionProperties();

        std::unordered_set<std::string> available;

        for (const auto& ext : availableExtensions)
        {
            available.insert(ext.extensionName);
        }

        for (const auto& extension : extensions)
        {
            if (available.contains(extension))
            {
                mExtensions.push_back(extension);
            }
            else
            {
                Console::LogWarning("vk::Instance extension not supported by system: {}", extension);
            }
        }
    }

    void VulkanInstance::SelectExtensions(const std::unordered_set<std::string>& whitelist, const std::unordered_set<std::string>& blacklist)
    {
        auto availableExtensions = vk::enumerateInstanceExtensionProperties();

        std::unordered_set<std::string> available;

        for (const auto& ext : availableExtensions)
        {
            available.insert(ext.extensionName);
        }

        for (const auto& available : availableExtensions)
        {
            std::string name = available.extensionName;

            bool whitelisted = whitelist.contains(name) or whitelist.empty();
            bool blacklisted = blacklist.contains(name);

            if (whitelisted and not blacklisted)
            {
                mExtensions.push_back(name);
            }
            else if (whitelisted and blacklisted)
            {
                Console::LogWarning("vk::Instance extension both whitelisted and blacklisted: {}. Ignoring extension", name);
            }
            else if (not whitelisted)
            {
                Console::LogWarning("vk::Instance extension not allowed by whitelist: {}. Ignoring extension", name);
            }
        }

        for (const auto& name : whitelist)
        {
            if (not available.contains(name))
            {
                Console::LogWarning("Whitelisted vk::Instance extension not supported by system: {}", name);
            }
        }
    }

    void VulkanInstance::SelectLayers(const std::unordered_set<std::string>& selected)
    {
        auto availableLayers = vk::enumerateInstanceLayerProperties();

        for (const auto& request : selected)
        {
            auto match = [&](const vk::LayerProperties& layer)
            {
                return request == layer.layerName;
            };

            auto found = std::find_if(availableLayers.begin(), availableLayers.end(), match);

            if (found != availableLayers.end())
            {
                mLayers.push_back(request);
            }
            else
            {
                Console::LogWarning("vk::Instance layer is not available: {}", request);
            }
        }
    }

    void VulkanInstance::Create()
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

        auto layers = getVectorCStrings(mLayers);
        auto extensions = getVectorCStrings(mExtensions);

        vk::InstanceCreateInfo instanceInfo = {
            {},
            &appInfo,
            static_cast<Types::UInt32>(layers.size()),
            layers.data(),
            static_cast<Types::UInt32>(extensions.size()),
            extensions.data()};

        mInstance = vk::createInstanceUnique(instanceInfo);
    }

    vk::Instance& VulkanInstance::Get()
    {
        return *mInstance;
    }
}