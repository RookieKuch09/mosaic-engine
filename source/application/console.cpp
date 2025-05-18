#include "application/console.hpp"

#include <chrono>

std::string Mosaic::Console::GetCurrentTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t nowClock = std::chrono::system_clock::to_time_t(now);
    std::tm localTime = *std::localtime(&nowClock);

    return std::format("[{:02}:{:02}:{:02}]", localTime.tm_hour, localTime.tm_min, localTime.tm_sec);
}
