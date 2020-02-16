/**
 * This module needs WinRing0 driver to be loaded first!
 */

#pragma once

#include <windows.h>

#include "cpu.hpp"
#include "DriverLoader.hpp"

namespace IntelCPU
{

class InfoGetter : public CPU::InfoGetter
{
public:
    InfoGetter();
    ~InfoGetter();

    const CPU::Info &GetInfo(UINT16 sampleTime) override;

private:
    UINT16 baseFreq;
};

} // namespace IntelCPU
