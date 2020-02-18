#include <cstdio>
#include <stdexcept>
#include <windows.h>

#include "memory.hpp"

using namespace Memory;

const Info &Memory::InfoGetter::GetInfo()
{
    MEMORYSTATUSEX mem;
    UINT64 totalMem = 0;
    mem.dwLength = sizeof(MEMORYSTATUSEX);
    if (GlobalMemoryStatusEx(&mem) && GetPhysicallyInstalledSystemMemory(&totalMem))
    {
        info.usage = mem.dwMemoryLoad;
        info.total = totalMem / 1024;
        info.used = info.total - (mem.ullAvailPhys / (1024 * 1024));
    }
    else
    {
        throw std::runtime_error("[Memory] Get memory info fail");
    }
    return info;
}

void Memory::InfoGetter::PrintInfo()
{
    printf("Memory:\t");
    printf("total: %ldMB\t", info.total);
    printf("used: %ldMB\t", info.used);
    printf("usage: %d%%\n", info.usage);
}