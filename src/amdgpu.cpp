#include <windows.h>
#include <cstdio>

#include "amdgpu.hpp"
#include "adl_defines.h"
#include "adl_structures.h"
#include "adl_sdk.h"

// Type of ADL functions.
typedef int (*ADL_MAIN_CONTROL_CREATE)(ADL_MAIN_MALLOC_CALLBACK, int);
typedef int (*ADL_MAIN_CONTROL_DESTROY)();
typedef int (*ADL_ADAPTER_NUMBEROFADAPTERS_GET)(int *);
typedef int (*ADL_ADAPTER_ADAPTERINFO_GET)(LPAdapterInfo, int);
typedef int (*ADL_OVERDRIVE6_THERMALCONTROLLER_CAPS)(int iAdapterIndex, ADLOD6ThermalControllerCaps *lpThermalControllerCaps);
typedef int (*ADL_OVERDRIVE6_TEMPERATURE_GET)(int iAdapterIndex, int *lpTemperature);

// Instance of ADL functions.
static ADL_MAIN_CONTROL_CREATE ADL_Main_Control_Create;
static ADL_MAIN_CONTROL_DESTROY ADL_Main_Control_Destroy;
static ADL_ADAPTER_NUMBEROFADAPTERS_GET ADL_Adapter_NumberOfAdapters_Get;
static ADL_ADAPTER_ADAPTERINFO_GET ADL_Adapter_AdapterInfo_Get;
static ADL_OVERDRIVE6_THERMALCONTROLLER_CAPS ADL_Overdrive6_ThermalController_Caps;
static ADL_OVERDRIVE6_TEMPERATURE_GET ADL_Overdrive6_Temperature_Get;

// Memory allocation function
void *__stdcall ADL_Main_Memory_Alloc(int iSize)
{
    void *lpBuffer = malloc(iSize);
    return lpBuffer;
}

// Optional Memory de-allocation function
void __stdcall ADL_Main_Memory_Free(void **lpBuffer)
{
    if (NULL != *lpBuffer)
    {
        free(*lpBuffer);
        *lpBuffer = NULL;
    }
}

// atiadlxx.dll
static HINSTANCE hDLL;

static bool init()
{
    puts("[AMDGPU] Init...");
    hDLL = LoadLibraryA("atiadlxx.dll");
    if (hDLL == NULL)
    {
        puts("[AMDGPU] No atiadlxx.dll!");
        return false;
    }

    ADL_Main_Control_Create = (ADL_MAIN_CONTROL_CREATE)GetProcAddress(hDLL, "ADL_Main_Control_Create");
    ADL_Main_Control_Destroy = (ADL_MAIN_CONTROL_DESTROY)GetProcAddress(hDLL, "ADL_Main_Control_Destroy");
    ADL_Adapter_NumberOfAdapters_Get = (ADL_ADAPTER_NUMBEROFADAPTERS_GET)GetProcAddress(hDLL, "ADL_Adapter_NumberOfAdapters_Get");
    ADL_Adapter_AdapterInfo_Get = (ADL_ADAPTER_ADAPTERINFO_GET)GetProcAddress(hDLL, "ADL_Adapter_AdapterInfo_Get");
    ADL_Overdrive6_ThermalController_Caps = (ADL_OVERDRIVE6_THERMALCONTROLLER_CAPS)GetProcAddress(hDLL, "ADL_Overdrive6_ThermalController_Caps");
    ADL_Overdrive6_Temperature_Get = (ADL_OVERDRIVE6_TEMPERATURE_GET)GetProcAddress(hDLL, "ADL_Overdrive6_Temperature_Get");

    puts("[AMDGPU] Done init");
    return true;
}

using namespace AMDGPU;

// Init this module.
bool AMDGPU::InfoGetter::initResult = init();

AMDGPU::InfoGetter::InfoGetter()
{
    if (!AMDGPU::InfoGetter::initResult)
    {
        puts("[AMDGPU] Module init fail!");
        exit(1);
    }

    // Init ADL
    if (ADL_OK != ADL_Main_Control_Create(ADL_Main_Memory_Alloc, 1))
    {
        puts("[AMDGPU] ADL init fail!");
        exit(1);
    }

    int n = 0;
    ADL_Adapter_NumberOfAdapters_Get(&n);
    printf("[AMDGPU] %d adapter(s).\n", n);
    AdapterInfo *adapterInfo = (AdapterInfo *)malloc(sizeof(AdapterInfo) * n);
    memset(adapterInfo, '\0', sizeof(AdapterInfo) * n);
    ADL_Adapter_AdapterInfo_Get(adapterInfo, sizeof(AdapterInfo) * n);
    for (int i = 0; i < n; i++)
    {
        if (adapterInfo[i].iPresent)
        {
            int adapterId = adapterInfo[i].iAdapterIndex;
            ADLOD6ThermalControllerCaps thermalControllerCaps = {0};
            if (ADL_OK != ADL_Overdrive6_ThermalController_Caps(adapterId, &thermalControllerCaps))
            {
                printf("[AMDGPU] Adapter %d Failed to get thermal controller capabilities!\n", adapterId);
                continue;
            }
            // Verifies that thermal controller exists on the GPU.
            if (ADL_OD6_TCCAPS_THERMAL_CONTROLLER != (thermalControllerCaps.iCapabilities & ADL_OD6_TCCAPS_THERMAL_CONTROLLER))
            {
                printf("[AMDGPU] Adapter %d don't support temperature\n", adapterId);
            }
            int temperature = 0;
            if (ADL_OK != ADL_Overdrive6_Temperature_Get(adapterId, &temperature))
            {
                printf("[AMDGPU] Adapter %d Failed to get GPU temperature!\n", adapterId);
                continue;
            }
            // The temperature is returned in mili-degree of Celsius
            printf("[AMDGPU] Adapter %d temperature is %d degrees celsius \n", adapterId, temperature / 1000);
        }
    }
    free(adapterInfo);
}

AMDGPU::InfoGetter::~InfoGetter()
{
}

const GPU::Info &AMDGPU::InfoGetter::Update()
{
    return this->info;
}

void AMDGPU::InfoGetter::Free()
{
    puts("[AMDGPU] called 'Free'");
}