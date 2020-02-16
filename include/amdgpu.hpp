#pragma once

#include <windows.h>

#include "gpu.hpp"

namespace AMDGPU
{

class InfoGetter : public GPU::InfoGetter
{
public:
    InfoGetter();
    ~InfoGetter();

    const GPU::Info& Update() override;
    void Free() override;

private:
    static bool initResult;
};

}