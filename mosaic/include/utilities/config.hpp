#pragma once

#include <toml++/toml.hpp>

namespace Mosaic
{
    enum class ConfigFiletype
    {
        TOML,
    };

    template <ConfigFiletype T>
    class ConfigFile
    {
    };

    template <>
    class ConfigFile<ConfigFiletype::TOML>
    {
    public:
        void Open(const std::string& path);
        void Save() const;

        template <typename T>
        T Get(const std::string& key) const;

        template <typename T>
        void Set(const std::string& key, const T& value);

    private:
        std::vector<std::string> SplitKey(const std::string& key) const;

        std::string mFilename;
        toml::table mData;
    };

    using TOMLFile = ConfigFile<ConfigFiletype::TOML>;
}

#include "../../inline/utilities/config.inl"