#include <cstdio>
#include <windows.h>

#include "systime.hpp"

int main()
{
    SetConsoleOutputCP(65001);
    
    SystemTime::Time time;
    time.Print();

    while (true)
    {
        Sleep(1000);
        time.Update();
        time.Print();
    }

    system("pause");
    return 0;
}
