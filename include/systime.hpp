#pragma once

#include <windows.h>

namespace SystemTime
{

class Time
{
public:
    Time();
    void Update();
    void Print();

    UINT16 year;
    UINT8 month;
    UINT8 day;
    UINT8 dayOfWeek;
    UINT8 hour;
    UINT8 minute;

private:
    SYSTEMTIME time;
};

}