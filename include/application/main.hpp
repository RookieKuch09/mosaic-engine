#pragma once

#include "application.hpp"

std::int32_t main()
{
    Mosaic::Application* application = Mosaic::CreateApplication();

    application->Setup();

    return application->Run();
}