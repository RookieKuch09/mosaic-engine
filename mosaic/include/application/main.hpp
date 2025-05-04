#pragma once

#include "application.hpp"

int main()
{
    Mosaic::Application* application = Mosaic::CreateApplication();

    return application->Run();
}