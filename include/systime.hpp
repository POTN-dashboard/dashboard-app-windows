#pragma once

#include <windows.h>

namespace SystemTime
{

class Time
{
public:
    static const UINT8 PACK_SIZE = 7;

    Time();
    void Update();
    void Serialize(BYTE *buf);
    void Print();

private:
    UINT16 year;
    UINT8 month;
    UINT8 day;
    UINT8 dayOfWeek;
    UINT8 hour;
    UINT8 minute;

    SYSTEMTIME time;
};

}