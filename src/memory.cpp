#include <cstdio>
#include <stdexcept>
#include <windows.h>

#include "memory.hpp"

using namespace Memory;

const Info &Memory::InfoGetter::GetInfo()
{
    MEMORYSTATUSEX mem;

    mem.dwLength = sizeof(MEMORYSTATUSEX);
    if (!GlobalMemoryStatusEx(&mem))
    {
        throw std::runtime_error("[Memory] Get memory info fail");
    }

    info.usage = mem.dwMemoryLoad;
    info.total = mem.ullTotalPhys / (1024 * 1024);
    info.used = (mem.ullTotalPhys - mem.ullAvailPhys) / (1024 * 1024);

    return info;
}

void Memory::InfoGetter::PrintInfo()
{
    printf("Memory:\t");
    printf("total: %ldMB\t", info.total);
    printf("used: %ldMB\t", info.used);
    printf("usage: %d%%\n", info.usage);
}