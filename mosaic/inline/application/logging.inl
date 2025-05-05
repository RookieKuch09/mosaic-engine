#pragma once

#include "../../include/application/console.hpp"

#include <format>
#include <iostream>

template <typename... Args>
void Mosaic::Console::LogSuccess(const std::string& message, Args&&... args)
{
    std::string timeStr = GetCurrentTime();
    std::cout << "\033[32m" << timeStr << " [SUCCESS]: \033[0m" << std::vformat(message, std::make_format_args(args...)) << std::endl;
}

template <typename... Args>
void Mosaic::Console::LogNotice(const std::string& message, Args&&... args)
{
    std::string timeStr = GetCurrentTime();
    std::cout << "\033[34m" << timeStr << " [NOTICE]: \033[0m" << std::vformat(message, std::make_format_args(args...)) << std::endl;
}

template <typename... Args>
void Mosaic::Console::LogError(const std::string& message, Args&&... args)
{
    std::string timeStr = GetCurrentTime();
    std::cout << "\033[31m" << timeStr << " [ERROR]: \033[0m" << std::vformat(message, std::make_format_args(args...)) << std::endl;

    throw;
}

template <typename... Args>
void Mosaic::Console::LogWarning(const std::string& message, Args&&... args)
{
    std::string timeStr = GetCurrentTime();
    std::cout << "\033[33m" << timeStr << " [WARNING]: \033[0m" << std::vformat(message, std::make_format_args(args...)) << std::endl;
}
