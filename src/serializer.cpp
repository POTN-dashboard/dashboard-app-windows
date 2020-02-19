#include "serializer.hpp"
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
        break;

    default:
        throw std::exception("[main] Unknown GPU");
    }

    // threads for getting info
    cpuInfoGetter = new std::thread(
        [&]() {
            CPU::Info info;
            while (!done)
            {
                info = cpu->GetInfo(1000);
                cpuInfoMutex.lock();
                cpuInfo = info;
                cpuInfoMutex.unlock();
            }
        });

    gpuInfoGetter = new std::thread(
        [&]() {
            GPU::Info info;
            while (!done)
            {
                info = gpu->GetInfo(1000);
                gpuInfoMutex.lock();
                gpuInfo = info;
                gpuInfoMutex.unlock();
            }
        });

    networkInfoGetter = new std::thread(
        [&]() {
            Network::Info info;
            while (!done)
            {
                info = network.GetInfo(1000);
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

    done = true;

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
    memcpy(nameBuf, CPU::InfoGetter::GetShortName(), 20);
    int nameLen = strlen(nameBuf);
    buf[3] = nameLen;
    memcpy(buf + 4, nameBuf, nameLen);

    // GPU name length, max is 20
    memset(nameBuf, 0, sizeof(nameBuf));
    memcpy(nameBuf, GPU::InfoGetter::GetShortName(), 20);
    nameLen = strlen(nameBuf);
    buf[24] = nameLen;
    memcpy(buf + 25, nameBuf, nameLen);
}

void Serializer::SerializeSystemInfo(BYTE *buf, int len)
{
    CPU::Info cpuInfo;
    GPU::Info gpuInfo;
    Network::Info networkInfo;

    cpuInfoMutex.lock();
    cpuInfo = this->cpuInfo;
    cpuInfoMutex.unlock();

    gpuInfoMutex.lock();
    gpuInfo = this->gpuInfo;
    gpuInfoMutex.unlock();

    networkInfoMutex.lock();
    networkInfo = this->networkInfo;
    networkInfoMutex.unlock();

    memset(buf, 0, len);

    // Package info
    buf[0] = USB::Connector::DATA_PACK;
    // CPU Freq
    buf[1] = 0;
    buf[2] = cpuInfo.freq >> 8;
    buf[3] = cpuInfo.freq;
    // CPU load
    buf[4] = cpuInfo.load;
    // CPU temp
    buf[5] = cpuInfo.temp;
    // GPU Freq
    buf[6] = 0;
    buf[7] = gpuInfo.GpuFreq >> 8;
    buf[8] = gpuInfo.GpuFreq;
    // GPU load
    buf[9] = gpuInfo.GpuLoad;
    // GPU temp
    buf[10] = gpuInfo.GpuTemp;
    // RAM
    double memUsed = mem.GetInfo().used / 1024.0;
    // RAM used, integer part (GB)
    buf[11] = memUsed;
    // RAM used, decimal part (GB)
    buf[12] = (memUsed - buf[11]) * 10;
    // VRAM
    double vramUsed = gpuInfo.MemUsed / 1024.0;
    // VRAM used, integer part (GB)
    buf[13] = vramUsed;
    // VRAM used, decimal part (GB)
    buf[14] = (vramUsed - buf[13]) * 10;
    // Network upload speed
    buf[15] = networkInfo.upSpeed >> 24;
    buf[16] = networkInfo.upSpeed >> 16;
    buf[17] = networkInfo.upSpeed >> 8;
    buf[18] = networkInfo.upSpeed;
    // Network download speed
    buf[19] = networkInfo.downSpeed >> 24;
    buf[20] = networkInfo.downSpeed >> 16;
    buf[21] = networkInfo.downSpeed >> 8;
    buf[22] = networkInfo.downSpeed;
    // Time
    time.Update();
    // Year
    buf[23] = time.year >> 8;
    buf[24] = time.year;
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
