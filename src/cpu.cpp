#include <cstdio>
#include <stdexcept>
#include <intrin.h>
#include <windows.h>
#include <powerbase.h>

#include "cpu.hpp"

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#pragma comment(lib, "PowrProf.lib")

using namespace CPU;

typedef struct
{
    ULONG Number;
    ULONG MaxMhz;
    ULONG CurrentMhz;
    ULONG MhzLimit;
    ULONG MaxIdleState;
    ULONG CurrentIdleState;
} PROCESSOR_POWER_INFORMATION;

Brand CPU::InfoGetter::GetBrand()
{
    static Brand brand = Brand::Uninit;

    if (brand != Brand::Uninit)
    {
        return brand;
    }

    int regs[4];
    __cpuid(regs, 0);

    static const int amdEbx =
        ((int)'h') << 24 | ((int)'t') << 16 | ((int)'u') << 8 | ((int)'A');

    static const int intelEbx =
        ((int)'u') << 24 | ((int)'n') << 16 | ((int)'e') << 8 | ((int)'G');

    if (regs[1] == amdEbx)
    {
        return CPU::AMD;
    }
    if (regs[1] == intelEbx)
    {
        return CPU::Intel;
    }
    return CPU::Unknown;
}

UINT8 CPU::InfoGetter::GetTotalProcessors()
{
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwNumberOfProcessors;
}

const char *CPU::InfoGetter::GetName()
{
    static char name[64];

    if (0 != name[0])
    {
        return name;
    }

    int cpu_info[4] = {-1};
    memset(name, '\0', sizeof(name));
    __cpuid(cpu_info, 0x80000000);
    unsigned n_ex_ids = cpu_info[0];
    for (int i = 0x80000000; i <= n_ex_ids; i++)
    {
        __cpuid(cpu_info, i);
        if (i == 0x80000002)
        {
            memcpy(name, cpu_info, sizeof(cpu_info));
        }
        else if (i == 0x80000003)
        {
            memcpy(name + 16, cpu_info, sizeof(cpu_info));
        }
        else if (i == 0x80000004)
        {
            memcpy(name + 32, cpu_info, sizeof(cpu_info));
        }
    }

    return name;
}

static inline bool isNumber(char c)
{
    return ('0' <= c && c <= '9') ? true : false;
}

const char *CPU::InfoGetter::GetShortName()
{
    static char shortNameBuf[32];
    static char *shortName = NULL;
    if (NULL != shortName)
    {
        return shortName;
    }

    Brand brand = GetBrand();
    const char *name = GetName();
    if (AMD == brand)
    {
        shortName = (char *)name;
    }
    else if (Intel == brand)
    {
        bool inParen = false;
        bool breakAfterSpace = false;
        int j = 0;
        for (int i = 0; name[i] != 0; i++)
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
            shortNameBuf[j++] = name[i];
        }
        // trim
        int i;
        for (i = 0; 0 != shortNameBuf[i]; i++)
        {
            if (' ' != shortNameBuf[i])
            {
                break;
            }
        }
        shortName = shortNameBuf + i + 6;
    }

    return shortName;
}

UINT16 CPU::InfoGetter::GetBaseFreq()
{
    UINT8 ncpus = GetTotalProcessors();
    UINT32 size = ncpus * sizeof(PROCESSOR_POWER_INFORMATION);
    PROCESSOR_POWER_INFORMATION *ppi = (PROCESSOR_POWER_INFORMATION *)MALLOC(size);
    LONG ret = CallNtPowerInformation(ProcessorInformation, NULL, 0, ppi, size);
    if (ret != 0)
    {
        throw std::runtime_error("[IntelCPU] CallNtPowerInformation syscall failed");
    }
    UINT16 baseFreq = ppi[0].MaxMhz;
    FREE(ppi);
    return baseFreq;
}

CPU::InfoGetter::InfoGetter()
{
    cpuUsageCounter = cpuUsagePdhQ.AddCounter(L"\\Processor Information(_Total)\\% Processor Time");
}

UINT8 CPU::InfoGetter::GetUsage(UINT16 sampleTime)
{
    cpuUsagePdhQ.Sample(sampleTime);
    cpuUsagePdhQ.GetValue(cpuUsageCounter, PDH::LONG | PDH::NOCAP100, &cpuUsageValue);
    return cpuUsageValue.longValue;
}

void CPU::InfoGetter::PrintBasicInfo()
{
    Brand brand = GetBrand();
    if (brand == AMD)
    {
        printf("Brand: AMD\n");
    }
    if (brand == Intel)
    {
        printf("Brand: Intel\n");
    }

    printf("Model: %s\n", GetName());
    printf("Processors: %d\n", GetTotalProcessors());
    printf("Base Frequency: %dMHz\n", GetBaseFreq());
}

void CPU::InfoGetter::PrintInfo()
{
    printf("CPU:\t");
    printf("freq: %dMHz\tusage: %d%%\ttemp: %dC\n", info.freq, info.load, info.temp);
}

CPU::InfoGetter::~InfoGetter()
{
}
