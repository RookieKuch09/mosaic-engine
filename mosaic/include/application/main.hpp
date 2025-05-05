#pragma once

#include "application.hpp"

int main()
{
    Mosaic::Application* application = Mosaic::CreateApplication();

    application->Setup();

    return application->Run();
}