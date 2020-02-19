#include <cstdio>
#include <iostream>
#include <exception>
#include <windows.h>
#include <locale>

// #include "gpu.hpp"

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "chs");

    try
    {
        
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    system("pause");
    return 0;
}
