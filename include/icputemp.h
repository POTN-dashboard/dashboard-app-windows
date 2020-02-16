#ifndef	__ICPUTEMP_H
#define	__ICPUTEMP_H

#ifdef	ICPUTEMP_EXPORTS
#define	ICPUTEMP_API __declspec(dllexport)
#else
#define	ICPUTEMP_API __declspec(dllimport)
#endif

#include <windows.h>

#ifdef __cplusplus
extern "C"
{
#endif

ICPUTEMP_API BOOL ICpuTemp_InitCPUTemp();
ICPUTEMP_API void ICpuTemp_Close();
ICPUTEMP_API WORD ICpuTemp_ReadCPUTemp();

#ifdef __cplusplus
}
#endif

#endif