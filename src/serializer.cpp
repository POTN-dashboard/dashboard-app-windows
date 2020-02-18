#include "Serializer.hpp"
#include "usb.hpp"

Serializer::Serializer()
{
    // CPU
    switch (CPU::InfoGetter::GetBrand())
    {
    case CPU::Brand::AMD:
        cpu = new AMDCPU::InfoGetter();
        break;

    case CPU::Brand::Intel:
        cpu = new IntelCPU::InfoGetter();
        break;

    default:
        throw std::exception("[main] Unknown CPU");
    }

    // GPU
    switch (GPU::InfoGetter::GetBrand())
    {
    case GPU::Brand::AMD:
        throw std::exception("[main] Unsupport GPU");
        break;

    case GPU::Brand::Intel:
        throw std::exception("[main] Unsupport GPU");
        break;

    case GPU::Brand::Nvidia:
        gpu = new NvidiaGPU::InfoGetter();

    default:
        throw std::exception("[main] Unknown CPU");
    }

    // threads for getting info
    cpuInfoGetter = new std::thread(
        [&]() {
            while (!cpuInfoGetterDone)
            {
                auto info = cpu->GetInfo(1000);
                cpuInfoMutex.lock();
                cpuInfo = info;
                cpuInfoMutex.unlock();
            }
        });

    gpuInfoGetter = new std::thread(
        [&]() {
            while (!gpuInfoGetterDone)
            {
                auto info = gpu->GetInfo(1000);
                gpuInfoMutex.lock();
                gpuInfo = info;
                gpuInfoMutex.unlock();
            }
        });

    networkInfoGetter = new std::thread(
        [&]() {
            while (!networkInfoGetterDone)
            {
                auto info = network.GetInfo(1000);
                networkInfoMutex.lock();
                networkInfo = info;
                networkInfoMutex.unlock();
            }
        });
}

Serializer::~Serializer()
{
    delete cpu;
    delete gpu;

    cpuInfoGetterDone = true;
    gpuInfoGetterDone = true;
    networkInfoGetterDone = true;

    cpuInfoGetter->join();
    gpuInfoGetter->join();
    networkInfoGetter->join();

    delete cpuInfoGetter;
    delete gpuInfoGetter;
    delete networkInfoGetter;
}

// Init info: CPU and GPU name, RAM and VRAM info
void Serializer::SerializeInitInfo(BYTE *buf, int len)
{
    memset(buf, 0, len);

    buf[0] = USB::Connector::CPU_GPU_PACK;

    // RAM = thisValue * 512 + 512 (MB)
    auto memInfo = mem.GetInfo();
    buf[1] = (memInfo.total - 512) / 512;

    // VRAM = thisValue * 128 + 128 (MB)
    auto gpuInfo = gpu->GetInfo(0);
    buf[2] = (gpuInfo.MemTotal - 128) / 128;

    // CPU name length, max is 20
    char nameBuf[21];
    memset(nameBuf, 0, sizeof(nameBuf));
    memcpy(nameBuf, CPU::InfoGetter::GetName(), 20);
    int nameLen = strlen(nameBuf);
    buf[3] = nameLen;
    memcpy(buf + 4, nameBuf, nameLen);

    // GPU name length, max is 20
    memset(nameBuf, 0, sizeof(nameBuf));
    memcpy(nameBuf, GPU::InfoGetter::GetName(), 20);
    nameLen = strlen(nameBuf);
    buf[24] = nameLen;
    memcpy(buf + 25, nameBuf, nameLen);
}

void Serializer::SerializeSystemInfo(BYTE *buf, int len)
{
    memset(buf, 0, len);

    buf[0] = USB::Connector::DATA_PACK;

    // ----------------- CPU ------------------------
    cpuInfoMutex.lock();
    // CPU Freq
    buf[1] = 0;
    buf[2] = (UINT8)(cpuInfo.freq >> 8);
    buf[3] = (UINT8)(cpuInfo.freq);
    // CPU load
    buf[4] = cpuInfo.load;
    // CPU temp
    buf[5] = cpuInfo.temp;
    cpuInfoMutex.unlock();

    // ----------------- GPU -------------------------
    gpuInfoMutex.lock();
    // GPU Freq
    buf[6] = 0;
    buf[7] = (UINT8)(gpuInfo.GpuFreq >> 8);
    buf[8] = (UINT8)(gpuInfo.GpuFreq);
    // GPU load
    buf[9] = gpuInfo.GpuLoad;
    // GPU temp
    buf[10] = gpuInfo.GpuTemp;
    // VRAM used, integer part (GB)
    double vramUsed = gpuInfo.MemUsed / 1024.0;
    buf[13] = (UINT8)(vramUsed);
    // VRAM used, decimal part (GB)
    buf[14] = (UINT8)((vramUsed - buf[13]) * 10);
    gpuInfoMutex.unlock();

    // ----------------- RAM ------------------------
    double memUsed = mem.GetInfo().used / 1024.0;
    // RAM used, integer part (GB)
    buf[11] = (UINT8)(memUsed);
    // RAM used, decimal part (GB)
    buf[12] = (UINT8)((memUsed - buf[11]) * 10);

    // --------------- Network -----------------------
    networkInfoMutex.lock();
    // Network upload speed
    UINT32 upSpeed = 260;
    buf[15] = (UINT8)(upSpeed >> 24);
    buf[16] = (UINT8)(upSpeed >> 16);
    buf[17] = (UINT8)(upSpeed >> 8);
    buf[18] = (UINT8)(upSpeed);
    // Network download speed
    UINT32 downSpeed = 2440;
    buf[19] = (UINT8)(downSpeed >> 24);
    buf[20] = (UINT8)(downSpeed >> 16);
    buf[21] = (UINT8)(downSpeed >> 8);
    buf[22] = (UINT8)(downSpeed);
    networkInfoMutex.unlock();

    // ----------------- Time ---------------------
    time.Update();
    // Year
    buf[23] = (UINT8)(time.year >> 8);
    buf[24] = (UINT8)(time.year);
    // Month
    buf[25] = time.month;
    // Day of week
    buf[26] = time.dayOfWeek;
    // Day
    buf[27] = time.day;
    // Hour
    buf[28] = time.hour;
    // Minute
    buf[29] = time.minute;
}