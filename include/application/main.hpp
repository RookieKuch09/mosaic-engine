#pragma once

#include "application.hpp"

int main()
{
    Application& application = CreateApplication();

    return application.Run();
}