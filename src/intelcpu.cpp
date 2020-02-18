#include <cstdio>
#include <stdexcept>
#include <string>
#include <windows.h>
#include <intrin.h>

#include "intelcpu.hpp"
#include "icputemp.h"
#include "OlsApi.h"
#include "OlsDef.h"

#define FREQ_SAMPLE_CORE 1

#pragma comment(lib, "WinRing0x64.lib")
#pragma comment(lib, "icputemp.lib")

using namespace IntelCPU;

static UINT16 getCpuCurFreq(UINT16 baseFreq, DWORD_PTR coreMask);

IntelCPU::InfoGetter::InfoGetter()
{
	if (!InitializeOls())
	{
		throw std::runtime_error("[IntelCPU] Init WinRing0 fail");
	}
	DWORD status = GetDllStatus();
	if (OLS_DLL_NO_ERROR != status)
	{
		throw std::runtime_error(std::string("[IntelCPU] Dll status error: ") + std::to_string(status));
	}

	baseFreq = CPU::InfoGetter::GetBaseFreq();

	if (!ICpuTemp_InitCPUTemp())
	{
		throw std::runtime_error("[IntelCPU] Init icputemp fail");
	}
}

IntelCPU::InfoGetter::~InfoGetter()
{
	DeinitializeOls();
	ICpuTemp_Close();
}

const CPU::Info &IntelCPU::InfoGetter::GetInfo(UINT16 sampleTime)
{
	info.freq = getCpuCurFreq(baseFreq, FREQ_SAMPLE_CORE);
	info.temp = (UINT8)ICpuTemp_ReadCPUTemp();
	info.load = this->GetUsage(sampleTime);
	return info;
}

static UINT16 getCpuCurFreq(UINT16 baseFreq, DWORD_PTR coreMask)
{
	SetThreadAffinityMask(GetCurrentThread(), coreMask);

	UINT64 aperf = 0, mperf = 0;
	DWORD *pMperf = (DWORD *)&mperf;
	DWORD *pAperf = (DWORD *)&aperf;

	WrmsrTx(231, 0, 0, coreMask);
	WrmsrTx(232, 0, 0, coreMask);

	for (int i = 1 << 16; i >= 0; i--)
		;

	RdmsrTx(231, pMperf, pMperf + 1, coreMask);
	RdmsrTx(232, pAperf, pAperf + 1, coreMask);

	return (UINT16)(((double)aperf / (double)mperf) * baseFreq);
}
