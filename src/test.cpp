#include <cstdio>
#include <iostream>
#include <exception>
#include <windows.h>

#include "gpu.hpp"

int main(int argc, char **argv)
{
    SetConsoleOutputCP(65001);

    try
    {
        GPU::InfoGetter::PrintBasicInfo();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    system("pause");
    return 0;
}
