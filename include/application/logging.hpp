#pragma once

#include <string>

namespace Mosaic
{
    std::string GetCurrentTime();

    template <typename... Args>
    void LogEvent(const std::string& message, Args&&... args);

    template <typename... Args>
    void LogError(const std::string& message, Args&&... args);

    template <typename... Args>
    void LogWarning(const std::string& message, Args&&... args);
}

#include "../../inline/application/logging.inl"
