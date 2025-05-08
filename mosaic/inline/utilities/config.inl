#pragma once

#include "../../include/utilities/config.hpp"

template <typename T>
T Mosaic::ConfigFile<Mosaic::ConfigFiletype::TOML>::Get(const std::string& key) const
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
                break;
            }
        }
        else
        {
            node = nullptr;

            break;
        }
    }

    if (node)
    {
        if (auto value = node->value<T>())
        {
            return *value;
        }
    }

    throw;
}

template <typename T>
void Mosaic::ConfigFile<Mosaic::ConfigFiletype::TOML>::Set(const std::string& key, const T& value)
{
    auto keys = SplitKey(key);
    toml::table* table = &mData;

    for (std::uint32_t index = 0; index < keys.size() - 1; index++)
    {
        if (not table->contains(keys[index]) or not(*table)[keys[index]].is_table())
        {
            table->insert_or_assign(keys[index], toml::table{});
        }

        table = (*table)[keys[index]].as_table();
    }

    table->insert_or_assign(keys.back(), value);
}