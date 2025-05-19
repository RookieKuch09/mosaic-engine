#pragma once

#include "utilities/config.hpp"

#include "application/console.hpp"

namespace Mosaic::Internal::Files
{
    template <typename T>
    T ConfigFile<ConfigFiletype::TOML>::Get(const std::string& key, const T& fallback) const
    {
        auto keys = SplitKey(key);

        const toml::node* node = &mData;

        for (const auto& key : keys)
        {
            if (auto table = node->as_table())
            {
                node = table->get(key);

                if (not node)
                {
                    return fallback;
                }
            }
            else
            {
                return fallback;
            }
        }

        if (node)
        {
            if (auto value = node->value<T>())
            {
                return *value;
            }
        }

        return fallback;
    }

    template <typename T, Types::UInt32 N>
    std::array<T, N> ConfigFile<ConfigFiletype::TOML>::Get(const std::string& key) const
    {
        auto keys = SplitKey(key);

        const toml::node* node = &mData;

        for (const auto& key : keys)
        {
            if (auto table = node->as_table())
            {
                node = table->get(key);

                if (not node)
                {
                    Console::Throw("Key \"{}\" does not exist in file \"{}\"", key, mFilename);
                }
            }
            else
            {
                Console::Throw("Key \"{}\" does not exist in file \"{}\"", key, mFilename);
            }
        }

        if (auto result = ExtractArray<T, N>(node))
        {
            return *result;
        }

        Console::Throw("Key \"{}\" does not exist in file \"{}\"", key, mFilename);

        throw;
    }

    template <typename T, Types::UInt32 N>
    std::array<T, N> ConfigFile<ConfigFiletype::TOML>::Get(const std::string& key, const std::array<T, N>& fallback) const
    {
        auto keys = SplitKey(key);

        const toml::node* node = &mData;

        for (const auto& key : keys)
        {
            if (auto table = node->as_table())
            {
                node = table->get(key);

                if (not node)
                {
                    return fallback;
                }
            }
            else
            {
                return fallback;
            }
        }

        if (auto result = ExtractArray<T, N>(node))
        {
            return *result;
        }

        return fallback;
    }

    template <typename T, Types::UInt32 N>
    std::optional<std::array<T, N>> ConfigFile<ConfigFiletype::TOML>::ExtractArray(const toml::node* node)
    {
        if (const auto* arr = node ? node->as_array() : nullptr)
        {
            if (arr->size() != N)
            {
                return std::nullopt;
            }

            std::array<T, N> result;

            for (Types::UInt32 index = 0; index < N; index++)
            {
                if (auto val = (*arr)[index].value<T>())
                {
                    result[index] = *val;
                }
                else
                {
                    return std::nullopt;
                }
            }

            return result;
        }

        return std::nullopt;
    }

    template <typename T>
    T ConfigFile<ConfigFiletype::TOML>::Get(const std::string& key) const
    {
        auto keys = SplitKey(key);

        const toml::node* node = &mData;

        for (const auto& key : keys)
        {
            if (auto table = node->as_table())
            {
                node = table->get(key);

                if (not node)
                {
                    Console::Throw("Key \"{}\" does not exist in file \"{}\"", key, mFilename);
                }
            }
            else
            {
                Console::Throw("Key \"{}\" does not exist in file \"{}\"", key, mFilename);
            }
        }

        if (node)
        {
            if (auto value = node->value<T>())
            {
                return *value;
            }
        }

        Console::Throw("Key \"{}\" does not exist in file \"{}\"", key, mFilename);

        throw;
    }

    template <typename T>
    void ConfigFile<ConfigFiletype::TOML>::Set(const std::string& key, const T& value)
    {
        auto keys = SplitKey(key);

        toml::table* table = &mData;

        for (Types::UInt32 index = 0; index < keys.size() - 1; index++)
        {
            if (not table->contains(keys[index]) or not(*table)[keys[index]].is_table())
            {
                table->insert_or_assign(keys[index], toml::table{});
            }

            table = (*table)[keys[index]].as_table();
        }

        table->insert_or_assign(keys.back(), value);
    }
}