#pragma once

#include <windows.h>

#include "pdh.hpp"

namespace CPU
{

enum Brand
{
    Uninit,
    AMD,
    Intel,
    Unknown,
};

typedef struct
{
    UINT16 freq; // MHz
    UINT8 load;  // %
    UINT8 temp;  // C
} Info;

class InfoGetter
{
public:
    static Brand GetBrand();
    static UINT8 GetTotalProcessors();
    static const char *GetName();
    static const char *GetShortName();
    static UINT16 GetBaseFreq();
    static void PrintBasicInfo();

    InfoGetter();
    virtual ~InfoGetter() = 0;
    virtual const Info &GetInfo(UINT16 sampleTime) = 0;
    UINT8 GetUsage(UINT16 sampleTime);
    void PrintInfo();

protected:
    Info info;

private:
    PDH::Query cpuUsagePdhQ;
    PDH::VALUE cpuUsageValue;
    PDH::COUNTER cpuUsageCounter;
};

} // namespace CPU