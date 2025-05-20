#pragma once

#include "utilities/numerics.hpp"

#include <optional>

#include <toml++/toml.hpp>

namespace Mosaic::Internal::Files
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
        ConfigFile() = default;
        ConfigFile(const std::string& path);

        void Open(const std::string& path);
        void Save() const;

        template <typename T>
        T Get(const std::string& key, const T& fallback) const;

        template <typename T, Types::UI32 N>
        std::array<T, N> Get(const std::string& key, const std::array<T, N>& fallback) const;

        template <typename T, Types::UI32 N>
        std::array<T, N> Get(const std::string& key) const;

        template <typename T>
        T Get(const std::string& key) const;

        template <typename T>
        void Set(const std::string& key, const T& value);

    private:
        static std::vector<std::string> SplitKey(const std::string& key);

        template <typename T, Types::UI32 N>
        static std::optional<std::array<T, N>> ExtractArray(const toml::node* node);

        std::string mFilename;
        toml::table mData;
    };

    using TOMLFile = ConfigFile<ConfigFiletype::TOML>;
}

#include "utilities/config.inl"