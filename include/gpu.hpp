#pragma once

#include <windows.h>

namespace GPU
{

enum Brand
{
    AMD,
    Intel,
    Nvidia,
    Unknown
};

typedef struct
{
    UINT8 GpuLoad;
    UINT8 GpuTemp;
    UINT16 GpuFreq;
    UINT16 MemTotal;
    UINT16 MemUsed;
    UINT8 MemLoad;
} Info;

class InfoGetter
{
public:
    static Brand GetBrand();
    static const char *GetName();
    static void PrintBasicInfo();

    virtual ~InfoGetter();
    void PrintInfo();
    virtual const Info &GetInfo(UINT16 sampleTime) = 0;

protected:
    Info info;
};

} // namespace GPU