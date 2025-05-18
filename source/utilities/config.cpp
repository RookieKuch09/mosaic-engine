#include "utilities/config.hpp"

Mosaic::ConfigFile<Mosaic::ConfigFiletype::TOML>::ConfigFile(const std::string& path)
{
    mFilename = path;

    mData = toml::parse_file(mFilename);
}

void Mosaic::ConfigFile<Mosaic::ConfigFiletype::TOML>::Open(const std::string& path)
{
    mFilename = path;

    mData = toml::parse_file(mFilename);
}

void Mosaic::ConfigFile<Mosaic::ConfigFiletype::TOML>::Save() const
{
    std::ofstream out(mFilename);

    out << mData;
}

std::vector<std::string> Mosaic::ConfigFile<Mosaic::ConfigFiletype::TOML>::SplitKey(const std::string& key)
{
    std::vector<std::string> result;

    std::uint64_t start = 0;
    std::uint64_t end = 0;

    while ((end = key.find('.', start)) != std::string::npos)
    {
        result.push_back(key.substr(start, end - start));

        start = end + 1;
    }

    result.push_back(key.substr(start));

    return result;
}