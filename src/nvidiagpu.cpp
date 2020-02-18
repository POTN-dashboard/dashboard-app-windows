#include <stdexcept>
#include <string>

#include "nvidiagpu.hpp"

#define NVML_LIB_PATH L"C:\\Program Files\\NVIDIA Corporation\\NVSMI\\nvml.dll"

using namespace NvidiaGPU;

NvidiaGPU::InfoGetter::InfoGetter()
{
    hNvml = LoadLibrary(NVML_LIB_PATH);
    if (hNvml == NULL)
    {
        throw std::runtime_error("[NvidiaGPU] Load nvml.dll fail");
    }

    nvmlErrorString = (FuncNvmlErrorString)GetProcAddress(hNvml, "nvmlErrorString");
    nvmlInit = (FuncNvmlInit)GetProcAddress(hNvml, "nvmlInit");
    nvmlShutdown = (FuncNvmlShutdown)GetProcAddress(hNvml, "nvmlShutdown");
    nvmlDeviceGetHandleByIndex = (FuncNvmlDeviceGetHandleByIndex)GetProcAddress(hNvml, "nvmlDeviceGetHandleByIndex");
    nvmlDeviceGetClockInfo = (FuncNvmlDeviceGetClockInfo)GetProcAddress(hNvml, "nvmlDeviceGetClockInfo");
    nvmlDeviceGetTemperature = (FuncNvmlDeviceGetTemperature)GetProcAddress(hNvml, "nvmlDeviceGetTemperature");
    nvmlDeviceGetUtilizationRates = (FuncNvmlDeviceGetUtilizationRates)GetProcAddress(hNvml, "nvmlDeviceGetUtilizationRates");
    nvmlDeviceGetMemoryInfo = (FuncNvmlDeviceGetMemoryInfo)GetProcAddress(hNvml, "nvmlDeviceGetMemoryInfo");

    nvmlReturn_t res;
    if (NVML_SUCCESS != (res = nvmlInit()))
    {
        throw std::runtime_error(std::string("Failed to initialize NVML: ") + nvmlErrorString(res));
    }
    res = nvmlDeviceGetHandleByIndex(0, &device);
    if (NVML_SUCCESS != res)
    {
        throw std::runtime_error(std::string("Failed to get handle for device 0: ") + nvmlErrorString(res));
    }
}

NvidiaGPU::InfoGetter::~InfoGetter()
{
    if (nvmlShutdown)
    {
        nvmlShutdown();
    }
    FreeLibrary(hNvml);
}

const GPU::Info &NvidiaGPU::InfoGetter::GetInfo(UINT16 sampleTime)
{
    if (0 != sampleTime)
    {
        Sleep(sampleTime);
    }
    nvmlReturn_t res;
    unsigned int gpuClock = 0;
    res = nvmlDeviceGetClockInfo(device, NVML_CLOCK_GRAPHICS, &gpuClock);
    if (NVML_SUCCESS != res)
    {
        goto error;
    }
    info.GpuFreq = gpuClock;

    unsigned int gpuTemp = 0;
    res = nvmlDeviceGetTemperature(device, NVML_TEMPERATURE_GPU, &gpuTemp);
    if (NVML_SUCCESS != res)
    {
        goto error;
    }
    info.GpuTemp = gpuTemp;

    nvmlUtilization_t utilization;
    res = nvmlDeviceGetUtilizationRates(device, &utilization);
    if (NVML_SUCCESS != res)
    {
        goto error;
    }
    info.GpuLoad = (UINT8)utilization.gpu;

    nvmlMemory_t mem;
    res = nvmlDeviceGetMemoryInfo(device, &mem);
    if (NVML_SUCCESS != res)
    {
        goto error;
    }
    info.MemTotal = mem.total / (1024 * 1024);
    info.MemUsed = mem.used / (1024 * 1024);
    info.MemLoad = (UINT8)((double)mem.used / (double)mem.total);
    return info;

error:
    throw std::runtime_error(std::string("[NvidiaGPU] GetInfo: ") + nvmlErrorString(res));
}