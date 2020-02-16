#pragma once

#include <windows.h>

#include "AMDCPU_ICPUEx.h"
#include "AMDCPU_IPlatform.h"
#include "AMDCPU_IDeviceManager.h"
#include "AMDCPU_IBIOSEx.h"
#include "cpu.hpp"
#include "DriverLoader.hpp"

#define DRIVER_FILE L"AMDRyzenMasterDriver.sys"
#define DRIVER_NAME L"AMDRyzenMaster"

namespace AMDCPU
{

class InfoGetter : public CPU::InfoGetter
{
public:
    InfoGetter();
    ~InfoGetter();

    const CPU::Info &GetInfo(UINT16 sampleTime) override;

private:
    DriverLoader driverLoader;
    IPlatform *platform;
    ICPUEx *cpuObj;
    CPUParameters stData;
};

} // namespace AMDCPU
