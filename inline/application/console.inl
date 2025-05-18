#pragma once

#include "application/console.hpp"

#include <format>
#include <iostream>

template <typename... Args>
void Mosaic::Console::LogSuccess(const std::string& message, Args&&... args)
{
    std::string time = GetCurrentTime();
    std::string content = std::vformat(message, std::make_format_args(args...));

    std::cout << std::vformat("\033[32m{} (SUCCESS): \033[0m{}\n", std::make_format_args(time, content));
}

template <typename... Args>
void Mosaic::Console::LogNotice(const std::string& message, Args&&... args)
{
    std::string time = GetCurrentTime();
    std::string content = std::vformat(message, std::make_format_args(args...));

    std::cout << std::vformat("\033[34m{} (NOTICE): \033[0m{}\n", std::make_format_args(time, content));
}

template <typename... Args>
void Mosaic::Console::LogWarning(const std::string& message, Args&&... args)
{
    std::string time = GetCurrentTime();
    std::string content = std::vformat(message, std::make_format_args(args...));

    std::cout << std::vformat("\033[33m{} (WARNING): \033[0m{}\n", std::make_format_args(time, content));
}

template <typename... Args>
void Mosaic::Console::LogError(const std::string& message, Args&&... args)
{
    std::string time = GetCurrentTime();
    std::string content = std::vformat(message, std::make_format_args(args...));

    std::cout << std::vformat("\033[31m{} (ERROR): \033[0m{}\n", std::make_format_args(time, content));
}

template <typename... Args>
void Mosaic::Console::Throw(const std::string& message, Args&&... args)
{
    std::string content = std::vformat(message, std::make_format_args(args...));

    throw std::runtime_error(content);
}