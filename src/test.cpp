#include <cstdio>
#include <iostream>
#include <exception>
#include <windows.h>

#include "memory.hpp"

static inline bool isNumber(char c)
{
    return ('0' <= c && c <= '9') ? true : false;
}

int main(int argc, char **argv)
{
    SetConsoleOutputCP(65001);

    try
    {
        const char *name = "Intel(R) Core(TM) i7-6700HQ CPU @ 2.60GHz";
        char shortName[21] = {0};
        bool inParen = false;
        bool breakAfterSpace = false;
        int j = 0;
        for (int i = 5; name[i] != 0; i++)
        {
            if ('(' == name[i])
            {
                inParen = true;
                continue;
            }
            if (')' == name[i])
            {
                inParen = false;
                continue;
            }
            if (inParen)
            {
                continue;
            }
            if (' ' == name[i] && breakAfterSpace)
            {
                break;
            }
            if (isNumber(name[i]))
            {
                breakAfterSpace = true;
            }
            shortName[j++] = name[i];
        }

        int i;
        for (i = 0; 0 != shortName[i]; i++)
        {
            if (' ' != shortName[i])
            {
                break;
            }
        }

        const char *res = shortName + i;
        puts(res);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    system("pause");
    return 0;
}
