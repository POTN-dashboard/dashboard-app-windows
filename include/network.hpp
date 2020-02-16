#pragma once

#include <windows.h>

#include "pdh.hpp"

namespace Network
{

typedef struct
{
    UINT32 upSpeed;   // KB/s
    UINT32 downSpeed; // KB/s
} Info;

class InfoGetter
{
public:
    const Info &GetInfo(UINT16 sampleTime);
    void PrintInfo();

private:
    Info info;
    WCHAR curAdapterName[128];
    DWORD oldAdapterIndex;
    WCHAR queryUpStr[200];
    WCHAR queryDownStr[200];
    PDH::COUNTER upCounter, downCounter;
    PDH::VALUE value;
    PDH::Query pdhQ;
};

} // namespace Network
