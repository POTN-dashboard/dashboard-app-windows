#include <cstdio>
#include <exception>
#include <windows.h>
#include <signal.h>

#include "memory.hpp"
#include "cpu.hpp"
#include "amdcpu.hpp"
#include "intelcpu.hpp"
#include "network.hpp"

CPU::InfoGetter *cpu;

void init();
void close();
void exitWith(int code);

int main()
{
    try
    {
        init();

        Memory::InfoGetter mem;
        Network::InfoGetter network;

        CPU::InfoGetter::PrintBasicInfo();
        printf("\n");

        while (true)
        {
            mem.GetInfo();
            mem.PrintInfo();
            cpu->GetInfo(1000);
            cpu->PrintInfo();
            network.GetInfo(1000);
            network.PrintInfo();
            Sleep(1000);
        }
    }
    catch (std::exception e)
    {
        puts(e.what());
        exitWith(EXIT_FAILURE);
    }

    system("pause");
    exitWith(EXIT_SUCCESS);
}

void init()
{
    signal(SIGABRT, exitWith);
    signal(SIGTERM, exitWith);
    signal(SIGINT, exitWith);
    signal(SIGSEGV, exitWith);
    signal(SIGILL, exitWith);
    signal(SIGFPE, exitWith);

    SetConsoleOutputCP(65001);

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
        puts("[main] Unknown CPU");
        exitWith(-1);
        break;
    }
}

void close()
{
    delete cpu;
}

void exitWith(int code)
{
    close();
    exit(code);
}