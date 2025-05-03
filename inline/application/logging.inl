#pragma once

#include "../../include/application/logging.hpp"

#include <ctime>
#include <format>
#include <iostream>

template <typename... Args>
void Mosaic::LogEvent(const std::string& message, Args&&... args)
{
    std::string timeStr = GetCurrentTime();
    std::cout << "\033[34m" << timeStr << "\033[0m" << std::vformat(message, std::make_format_args(args...)) << std::endl;
}

template <typename... Args>
void Mosaic::LogError(const std::string& message, Args&&... args)
{
    std::string timeStr = GetCurrentTime();
    std::cout << "\033[31m" << timeStr << "\033[0m" << std::vformat(message, std::make_format_args(args...)) << std::endl;

    throw;
}

template <typename... Args>
void Mosaic::LogWarning(const std::string& message, Args&&... args)
{
    std::string timeStr = GetCurrentTime();
    std::cout << "\033[33m" << timeStr << "\033[0m" << std::vformat(message, std::make_format_args(args...)) << std::endl;
}
