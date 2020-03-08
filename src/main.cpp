#include <cstdio>
#include <exception>
#include <windows.h>
#include <signal.h>
#include <thread>
#include <mutex>
#include <locale>

#include "serializer.hpp"
#include "usb.hpp"

void init();
void cleanAndExit(int sig);
void controlLoop(USB::Connector &usb);
bool waitForReady(USB::Connector &usb);
bool sendInitInfo(USB::Connector &usb);
bool sendSysInfoLoop(USB::Connector &usb);

Serializer *serializer;

int main()
{
    try
    {
        init();
        USB::Connector usb;
        controlLoop(usb);
    }
    catch (const std::exception &e)     //捕获异常
    {
        puts(e.what());
    }

    cleanAndExit(0);
}

void init()
{
    signal(SIGABRT, cleanAndExit);  // 异常终止 abort
    signal(SIGTERM, cleanAndExit);  //终止
    signal(SIGINT, cleanAndExit);   //CTRL+C Interrupt from keyboard
    signal(SIGSEGV, cleanAndExit);  //访问无效内存 或没有权限的内存 
    signal(SIGILL, cleanAndExit);   //指令中有非法指令
    signal(SIGFPE, cleanAndExit);   // 浮点运算错误

    setlocale(LC_ALL, "chs");      //地域设置，编码ANSI 中文不会乱码

    serializer = new Serializer();
}

void cleanAndExit(int sig)
{
    delete serializer;
    system("pause");       // DOS调用 黑框框不会闪退 
    exit(sig);              
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
        if (!sendInitInfo(usb))
        {
            puts("Device disconnected!");
            Sleep(1000);
            continue;
        }
        puts("CPU GPU info sent!");
        if (!sendSysInfoLoop(usb))
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

bool sendInitInfo(USB::Connector &usb)
{
    BYTE cpuGpuInfo[USB::Connector::MAX_PACK_SIZE];
    BYTE readBuf[USB::Connector::MAX_PACK_SIZE];
    serializer->SerializeInitInfo(cpuGpuInfo, sizeof(cpuGpuInfo));
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

bool sendSysInfoLoop(USB::Connector &usb)
{
    BYTE buf[USB::Connector::MAX_PACK_SIZE];
    while (true)
    {
        serializer->SerializeSystemInfo(buf, sizeof(buf));
        if (USB::Error::DISCONNECTED == usb.Write(buf, sizeof(buf)))
        {
            return false;
        }
        Sleep(1000);
    }
}
