#pragma once

#include <string>

namespace Mosaic
{
    class Console
    {
    public:
        template <typename... Args>
        void LogSuccess(const std::string& message, Args&&... args);

        template <typename... Args>
        void LogNotice(const std::string& message, Args&&... args);

        template <typename... Args>
        void LogError(const std::string& message, Args&&... args);

        template <typename... Args>
        void LogWarning(const std::string& message, Args&&... args);

    private:
        std::string GetCurrentTime();
    };
}

#include "../../inline/application/logging.inl"
