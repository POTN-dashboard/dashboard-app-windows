#include <cstdio>
#include <iostream>
#include <exception>
#include <windows.h>

#include "usb.hpp"

void controlLoop(USB::Connector &usb);
bool waitForReady(USB::Connector &usb);
bool sendCpuGpuInfo(USB::Connector &usb);
bool sendDataLoop(USB::Connector &usb);
void serializeCpuGpuInfo(BYTE *buf, int len);
void serializeComputerInfo(BYTE *buf, int len);

int main(int argc, char **argv)
{
    SetConsoleOutputCP(65001);

    try
    {
        USB::Connector usb;
        controlLoop(usb);
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
    system("pause");
    return 0;
}

void controlLoop(USB::Connector &usb)
{
    while (true)
    {
        if (!usb.Connect())
        {
            puts("Waiting for device...");
            Sleep(1000);
            continue;
        }
        puts("Device connected!");
        if (!waitForReady(usb))
        {
            puts("Device disconnected!");
            Sleep(1000);
            continue;
        }
        puts("Device ready!");
        if (!sendCpuGpuInfo(usb))
        {
            puts("Device disconnected!");
            Sleep(1000);
            continue;
        }
        puts("CPU GPU info sent!");
        if (!sendDataLoop(usb))
        {
            puts("Device disconnected!");
            Sleep(1000);
            continue;
        }
    }
}

bool waitForReady(USB::Connector &usb)
{
    BYTE buf[USB::Connector::MAX_PACK_SIZE];
    while (true)
    {
        Sleep(1000);
        int actualLen = 0;
        USB::Error err = usb.Read(buf, sizeof(buf), &actualLen);
        if (USB::Error::DISCONNECTED == err)
        {
            return false;
        }
        if (USB::Error::OK != err)
        {
            continue;
        }
        if (buf[0] = USB::Connector::READY_PACK)
        {
            return true;
        }
    }
}

bool sendCpuGpuInfo(USB::Connector &usb)
{
    BYTE cpuGpuInfo[USB::Connector::MAX_PACK_SIZE];
    BYTE readBuf[USB::Connector::MAX_PACK_SIZE];
    serializeCpuGpuInfo(cpuGpuInfo, sizeof(cpuGpuInfo));
    while (true)
    {
        Sleep(1000);
        USB::Error err = usb.Write(cpuGpuInfo, sizeof(cpuGpuInfo));
        if (err == USB::Error::DISCONNECTED)
        {
            return false;
        }
        if (USB::Error::OK != err)
        {
            continue;
        }

        // wait for ack
        int actualLen = 0;
        err = usb.Read(readBuf, sizeof(readBuf), &actualLen);
        if (USB::Error::DISCONNECTED == err)
        {
            return false;
        }
        if (USB::Error::OK != err)
        {
            continue;
        }
        if (readBuf[0] == USB::Connector::ACK_PACK)
        {
            return true;
        }
    }
}

void serializeCpuGpuInfo(BYTE *buf, int len)
{
    memset(buf, 0, len);

    buf[0] = USB::Connector::CPU_GPU_PACK;

    // RAM = thisValue * 512 + 512 (MB)
    buf[1] = 1;

    // VRAM = thisValue * 128 + 128 (MB)
    buf[2] = 15;

    // CPU name length, max is 20
    const char *cpuName = "Ryzen 5 3500U";
    int cpuNameLen = strlen(cpuName);
    buf[3] = cpuNameLen;
    memcpy(buf + 4, cpuName, cpuNameLen);

    // GPU name length, max is 20
    const char *gpuName = "GeForce RTX 2080 Ti";
    int gpuNameLen = strlen(gpuName);
    buf[24] = gpuNameLen;
    memcpy(buf + 25, gpuName, gpuNameLen);
}

bool sendDataLoop(USB::Connector &usb)
{
    BYTE buf[USB::Connector::MAX_PACK_SIZE];
    while (true)
    {
        serializeComputerInfo(buf, sizeof(buf));
        if (USB::Error::DISCONNECTED == usb.Write(buf, sizeof(buf)))
        {
            return false;
        }
        Sleep(1000);
    }
}

void serializeComputerInfo(BYTE *buf, int len)
{
    // for testing
    static UINT8 cpuTemp = 50;

    memset(buf, 0, len);

    buf[0] = USB::Connector::DATA_PACK;

    // CPU Freq
    UINT16 cpuFreq = 3400;
    buf[1] = 0;
    buf[2] = (UINT8)(cpuFreq >> 8);
    buf[3] = (UINT8)(cpuFreq);
    // CPU load
    buf[4] = 32;
    // CPU temp
    buf[5] = cpuTemp++;

    // GPU Freq
    UINT16 gpuFreq = 1200;
    buf[6] = 0;
    buf[7] = (UINT8)(gpuFreq >> 8);
    buf[8] = (UINT8)(gpuFreq);
    // GPU load
    buf[9] = 26;
    // GPU temp
    buf[10] = 45;

    // RAM used (整数位)
    buf[11] = 4;
    // RAM used (小数位)
    buf[12] = 3;

    // VRAM used (整数位)
    buf[13] = 2;
    // VRAM used (小数位)
    buf[14] = 3;

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

    // Year
    UINT16 year = 2020;
    buf[23] = (UINT8)(year >> 8);
    buf[24] = (UINT8)(year);
    // Month
    buf[25] = 1;
    // Day of week
    buf[26] = 1;
    // Day
    buf[27] = 1;
    // Hour
    buf[28] = 1;
    // Minute
    buf[29] = 1;
}