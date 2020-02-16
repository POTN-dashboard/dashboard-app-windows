#include <windows.h>
#include <stdexcept>

#include "amdcpu.hpp"

#pragma comment(lib, "Device.lib")
#pragma comment(lib, "Platform.lib")

using namespace AMDCPU;

AMDCPU::InfoGetter::InfoGetter()
{
    driverLoader.InitSvc(DRIVER_FILE, DRIVER_NAME, DRIVER_NAME, SERVICE_DEMAND_START);
    if (DriverLoader::OK != driverLoader.CreateSvc())
    {
        driverLoader.UnloadSvc();
        throw std::runtime_error("[AMDCPU] Create driver service fail");
    }
    if (DriverLoader::OK != driverLoader.StartSvc())
    {
        driverLoader.UnloadSvc();
        throw std::runtime_error("[AMDCPU] Start driver service fail");
    }

    platform = &GetPlatform();
    if (!platform->Init())
    {
        throw std::runtime_error("[AMDCPU] Platform init fail");
    }
    IDeviceManager &rDeviceManager = platform->GetIDeviceManager();
    cpuObj = (ICPUEx *)rDeviceManager.GetDevice(dtCPU, 0);
}

AMDCPU::InfoGetter::~InfoGetter()
{
    if (platform != NULL)
    {
        platform->UnInit();
    }
}

const CPU::Info &AMDCPU::InfoGetter::GetInfo(UINT16 sampleTime)
{
    cpuObj->GetCPUParameters(stData);
    info.freq = (UINT16) stData.dPeakSpeed;
    info.temp = (UINT8) stData.dTemperature;
    info.usage = this->GetUsage(sampleTime);
    return info;
}
