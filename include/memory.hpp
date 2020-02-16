#pragma once

#include <windows.h>

namespace Memory
{

typedef struct
{
    UINT32 total; // MB
    UINT32 used;  // MB
    UINT8 usage;  // %
} Info;

class InfoGetter
{
public:
    const Info &GetInfo();
    void PrintInfo();

private:
    Info info;
};

} // namespace Memory
