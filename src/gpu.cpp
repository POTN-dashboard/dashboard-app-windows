#include <windows.h>
#include <cstdio>
#include <cstring>

#include "gpu.hpp"

#define DEVICE_NAME_LEN 128

#pragma comment(lib, "User32.lib")

using namespace GPU;

const char *GPU::InfoGetter::GetName()
{
    static char deviceName[DEVICE_NAME_LEN];
    if (deviceName[0] != 0)
    {
        return deviceName;
    }
    DISPLAY_DEVICEA device;
    int displayIndex = 0;
    device.cb = sizeof(device);
    while (EnumDisplayDevicesA(0, displayIndex, &device, 0))
    {
        if (device.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE)
        {
            break;
        }
        displayIndex++;
    }
    memcpy(deviceName, device.DeviceString, DEVICE_NAME_LEN);
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
    static Brand brand = Brand::Uninit;
    if (brand != Brand::Uninit)
    {
        return brand;
    }
    const char *deviceName = GetName();
    char deviceNameLower[DEVICE_NAME_LEN];
    for (int i = 0; i < DEVICE_NAME_LEN; i++)
    {
        deviceNameLower[i] = tolower(deviceName[i]);
    }
    if (strstr(deviceNameLower, "amd"))
    {
        return Brand::AMD;
    }
    if (strstr(deviceNameLower, "intel"))
    {
        return Brand::Intel;
    }
    if (strstr(deviceNameLower, "nvidia"))
    {
        return Brand::Nvidia;
    }
    return Brand::Unknown;
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
