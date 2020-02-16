#pragma once

#include "gpu.hpp"
#include "nvml.h"

namespace NvidiaGPU
{

typedef const char *(*FuncNvmlErrorString)(nvmlReturn_t result);
typedef nvmlReturn_t (*FuncNvmlInit)();
typedef nvmlReturn_t (*FuncNvmlShutdown)();
typedef nvmlReturn_t (*FuncNvmlDeviceGetHandleByIndex)(unsigned int index, nvmlDevice_t *device);
typedef nvmlReturn_t (*FuncNvmlDeviceGetClockInfo)(nvmlDevice_t device, nvmlClockType_t type, unsigned int *clock);
typedef nvmlReturn_t (*FuncNvmlDeviceGetTemperature)(nvmlDevice_t device, nvmlTemperatureSensors_t sensorType, unsigned int *temp);
typedef nvmlReturn_t (*FuncNvmlDeviceGetUtilizationRates)(nvmlDevice_t device, nvmlUtilization_t *utilization);
typedef nvmlReturn_t (*FuncNvmlDeviceGetMemoryInfo)(nvmlDevice_t device, nvmlMemory_t *memory);

class InfoGetter : public GPU::InfoGetter
{
public:
    InfoGetter();
    ~InfoGetter();

    const GPU::Info &GetInfo(UINT16 sampleTime) override;

private:
    HMODULE hNvml;

    FuncNvmlErrorString nvmlErrorString;
    FuncNvmlInit nvmlInit;
    FuncNvmlShutdown nvmlShutdown;
    FuncNvmlDeviceGetHandleByIndex nvmlDeviceGetHandleByIndex;
    FuncNvmlDeviceGetClockInfo nvmlDeviceGetClockInfo;
    FuncNvmlDeviceGetTemperature nvmlDeviceGetTemperature;
    FuncNvmlDeviceGetUtilizationRates nvmlDeviceGetUtilizationRates;
    FuncNvmlDeviceGetMemoryInfo nvmlDeviceGetMemoryInfo;

    nvmlDevice_t device;
};

} // namespace NvidiaGPU
