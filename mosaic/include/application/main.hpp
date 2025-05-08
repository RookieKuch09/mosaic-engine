#pragma once

#include "application.hpp"

#if defined(MACOS) || defined(LINUX)
std::int32_t main()
{
    Mosaic::Application* application = Mosaic::CreateApplication();

    application->Setup();

    return application->Run();
}
#elif defined(WINDOWS)
std::int32_t wWinMain(HINSTANCE, HINSTANCE, PWSTR, int)
{
    FILE* fp;
    freopen_s(&fp, "CONOUT$", "w", stdout);
    freopen_s(&fp, "CONOUT$", "w", stderr);
    freopen_s(&fp, "CONIN$", "r", stdin);

    Mosaic::Application* application = Mosaic::CreateApplication();

    application->Setup();

    return application->Run();
}
#endif