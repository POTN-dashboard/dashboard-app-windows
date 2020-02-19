#include <windows.h>
#include <cstdio>
#include <d3d9.h>

#include "initializer.h"
#include "gpu.hpp"

#define DEVICE_NAME_LEN 127

#pragma comment(lib, "d3d9.lib")

using namespace GPU;

static Brand brand = Brand::Uninit;
static char deviceNameBuf[DEVICE_NAME_LEN + 1];
static char *deviceName = nullptr;

INITIALIZER(init)
{
    IDirect3D9 *d3dobject = Direct3DCreate9(D3D_SDK_VERSION);
    D3DADAPTER_IDENTIFIER9 identifier;
    d3dobject->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0, &identifier);
    memcpy(deviceNameBuf, identifier.Description, DEVICE_NAME_LEN);
    deviceName = deviceNameBuf;
    switch (identifier.VendorId)
    {
    case 0x1002:
        brand = Brand::AMD;
        break;

    case 0x8086:
        brand = Brand::Intel;
        break;

    case 0x10DE:
        brand = Brand::Nvidia;
        break;

    default:
        brand = Brand::Unknown;
    }

    d3dobject->Release();
}

const char *GPU::InfoGetter::GetName()
{
    return deviceName;
}

const char *GPU::InfoGetter::GetShortName()
{
    static char shortNameBuf[32];
    static char *shortName = NULL;

    if (NULL != shortName)
    {
        return shortName;
    }
    const char *name = GetName();
    Brand brand = GetBrand();
    if (AMD == brand)
    {
        shortName = (char *)name;
    }
    else if (Intel == brand)
    {
        shortName = (char *)name;
    }
    else if (Nvidia == brand)
    {
        shortName = (char *)name + 15;
    }

    return shortName;
}

Brand GPU::InfoGetter::GetBrand()
{
    return brand;
}

GPU::InfoGetter::~InfoGetter()
{
}

void GPU::InfoGetter::PrintBasicInfo()
{
    const char *brandName;
    switch (GetBrand())
    {
    case Brand::AMD:
        brandName = "AMD";
        break;

    case Brand::Intel:
        brandName = "Intel";
        break;

    case Brand::Nvidia:
        brandName = "Nvidia";
        break;

    default:
        brandName = "Unknown";
        break;
    }
    printf("GPU Brand: %s\n", brandName);
    printf("GPU Name: %s\n", GetName());
}

void GPU::InfoGetter::PrintInfo()
{
    printf("GPU Core:\t");
    printf("freq: %dMHz\t", info.GpuFreq);
    printf("temp: %dC\t", info.GpuTemp);
    printf("load: %d%%\n", info.GpuLoad);
    printf("GPU Memory:\t");
    printf("total: %dMB\t", info.MemTotal);
    printf("used: %dMB\t", info.MemUsed);
    printf("load: %d%%\n", info.MemLoad);
}
