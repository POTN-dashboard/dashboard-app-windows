//===----------------------------------------------------------------------===//
//====  Copyright (c) 2020 Advanced Micro Devices, Inc.  All rights reserved.
//
//               Developed by: Advanced Micro Devices, Inc.

/*! \mainpage
*
*  \tableofcontents
*
* \section intro_sec Introduction
* The AMD Ryzen Master Monitoring Software Development Kit (SDK) provides the ability to Read the Parameters of certain AMD processors, sockets, and chipsets.
* This document describes the functional reference to the application programming interface (API). The API is a toolkit of programming functions that allows software developers to Read certain CPU and Memory Parameters to be displayed in their tool.
*
* <br>
* Primary features available in the SDK are
* - Reading CPU Parameters
* - Reading Memory Parameters
* <br>
*
* \section install Installation
* AMD Ryzen Master Monitoring SDK has different components. The AMDRyzenMasterMonitoringSDK.exe will install these components at default locations and sets environment variable in the computer as described below.
* - Environment Variable
*      -# "%AMDRMMONITORSDKPATH%" environment variable is set
*      -# It is set to installation path: by default - AMDRMMONITORSDKPATH="C:\Program Files\AMD\RyzenMasterMonitoringSDK\"
* - Driver
*      -# The driver component used to communicate with the underlying hardware is installed and run as a service
*      -# Default path: "%AMDRMMONITORSDKPATH%\bin"
* - Library
*      -# The API implementation files (dlls, libs, header files)
*      -# Default path: "%AMDRMMONITORSDKPATH%\bin", "%AMDRMMONITORSDKPATH%\lib", "%AMDRMMONITORSDKPATH%\include"
* - Documentation
*	-# A detailed documentation of the APIs is available in the "%AMDRMMONITORSDKPATH%\Documentation" folder
* - Console Application
*      -# CPU AMDRyzenMasterMonitoringSampleApp CLI demonstrates the usage of CPU Monitoring SDK APIs
*      -# Default path: "C:\Program Files\AMD\RyzenMasterMonitoringSDK\AMDRyzenMasterMonitoringSampleApp"

* \section run Running the Command-line Application(AMDRyzenMasterMonitoringSampleApp.exe)
* - Start a command prompt as an administrator
* - AMDRyzenMasterMonitoringSampleApp.exe available in the AMDRyzenMasterMonitoringSampleApp\ bin-prebuilt folder can be executed to Get CPU and Memory Parameters

* \section build Building the Console Application
* -The Console Application can be built using Visual Studio solution file(compatible with Visual Studio 2015) available in the AMDRyzenMasterMonitoringSampleApp folder
* -"%AMDRMMONITORSDKPATH%" environment variable set to the root folder of the SDK will be used to locate header files and libraries required to build Console Application

* \section support Supported AMD CPU
* Currently the APIs provided in the SDK are supported on AMD RYZEN processors
* -AMD Family 17h Processor

* \section supports Supported Operating Systems
* Currently the APIs provided in the SDK are supported on following operating systems
* -Microsoft Windows 10 64 - bit version

* \subsection  BIOSDepend	APIs dependent on BIOS

 @ref

	- IBIOSEx::GetMemVDDIO()
	- IBIOSEx::GetCurrentMemClock()
	- IBIOSEx::GetMemCtrlTcl()
	- IBIOSEx::GetMemCtrlTrcdrd()
	- IBIOSEx::GetMemCtrlTras()
	- IBIOSEx::GetMemCtrlTrp()
	- IBIOSEx::GetVersion()
	- IBIOSEx::GetVendor()
	- IBIOSEx::GetDate()

* \subsection	CPUDepend	APIs dependent on CPU

 @ref

	- ICPUEx::GetName()
	- ICPUEx::GetDescription()
	- ICPUEx::GetVendor()
	- ICPUEx::GetRole()
	- ICPUEx::GetClassName()
	- ICPUEx::GetType()
	- ICPUEx::GetIndex()
	- ICPUEx::GetL1DataCache()
	- ICPUEx::GetL1InstructionCache()
	- ICPUEx::GetL2Cache()
	- ICPUEx::GetL3Cache()
	- ICPUEx::GetCoreCount()
	- ICPUEx::GetCorePark()
	- ICPUEx::GetPackage()
	- ICPUEx::GetCPUParameters()
*/
#ifndef _AMDCPU_ICPUEx_H_
#define _AMDCPU_ICPUEx_H_

#include "AMDCPU_IDevice.h"

class ICPUEx : public IDevice
{
public:
/**
* Gets the Processor Name String Identifier as obtained from CPUID instruction CPUID_Fn80000002, CPUID_Fn80000003 and CPUID_Fn80000004.
* @param void
* @return Returns Processor Name String Identifier.
*/
	virtual const wchar_t* GetName(void) = 0;

/**
* Gets the device description which includes BaseFamily, ExtendedFamily, BaseModel, ExtendedModel, Stepping detail as obtained from CPUID instruction CPUID_Fn00000001_EAX.
* @param void
* @return Returns device description string.
*/
	virtual const wchar_t* GetDescription(void) = 0;

/**
* Gets the device vendor name as obtained from CPUID instruction CPUID_Fn00000000_EBX, CPUID_Fn00000000_ECX and CPUID_Fn00000000_EDX.
* @param void
* @return Returns device vendor name in string format.
*/
	virtual const wchar_t* GetVendor(void) = 0;

/**
* Gets the role of the device.
* @param void
* @return Returns role of the device in string format.
*/
	virtual const wchar_t* GetRole(void) = 0;

/**
* Gets the device category name.
* @param void
* @return Returns device category name in string format.
*/
	virtual const wchar_t* GetClassName(void) = 0;

/**
* Gets the device type. Presently only CPU device is supported.
* @param void
* @return Returns the device type.
*/
	virtual AOD_DEVICE_TYPE GetType(void) = 0;

/**
* Gets the index of device instance.
* @param void
* @return Returns device index of the device.
*/
	virtual unsigned long GetIndex(void) = 0;

/**
* Gets the L1 data cache information as obtained from CPUID instruction CPUID_Fn80000005_ECX.
* @param L1DataCache [out]  Address of variable of struct CACHE_INFO which will hold value of L1DataCache
* @return
*	- 0  - if successful.
*	- -1 - if failure.
*/
	virtual int GetL1DataCache(CACHE_INFO& L1DataCache) = 0;

/**
* Gets the L1 instruction cache information as obtained from CPUID instruction CPUID_Fn80000005_EDX.
* @param L1InstCache [out]  Address of variable of struct CACHE_INFO which will hold value of L1Instruction Cache.
* @return
*	- 0  - if successful.
*	- -1 - if failure.
*/
	virtual int GetL1InstructionCache(CACHE_INFO& L1InstCache) = 0;

/**
* Gets the L2 cache information as obtained from CPUID instruction CPUID_Fn80000006_ECX.
* @param L2Cache [out]  Address of variable of struct CACHE_INFO which will hold value of L2 Cache.
* @return
*	- 0  - if successful.
*	- -1 - if failure.
*/
	virtual int GetL2Cache(CACHE_INFO& L2Cache) = 0;

/**
* Gets the L3 cache information as obtained from CPUID instruction CPUID_Fn80000006_EDX.
* @param L3Cache [out]  Address of variable of struct CACHE_INFO which will hold value of L3 Cache.
* @return
*	- 0  - if successful.
*	- -1 - if failure.
*/
	virtual int GetL3Cache(CACHE_INFO& L3Cache) = 0;

/**
* Gets the physical core count of the processor
* @param uVal [out] Address of unsigned int which will hold the physical core count of the processor
* @return
*	- 0  - if successful.
*	- -1 - if failure.
*/
	virtual int GetCoreCount(unsigned int& uVal) = 0;

/**
* Gets the status of Current Core Parking
* @param uVal [out] Address of unsigned integer variable which will hold number of parked cores.
* @return
*	- 0  - if successful.
*	- -1 - if failure.
*/
	virtual int GetCorePark(unsigned int& uVal) = 0;
	
/**
* Gets processor package information as Socket AM4, Socket FP5, Socket SP3R2 etc.
* @param void
* @return Returns processor package information as string identifier.
*/
	virtual const wchar_t* GetPackage(void) = 0;

/**
*	Get CPU Parameters Data such as current and power monitoring data, Current OC Mode, Effective Frequency details.
*	@param	stInfo [out] CPUParameters structure having power related information, Current OC Mode, Effective Freq etc.
*	@return Status
*	 -  -1 --> Failure
*	 -   0 --> Success
*	 -   4 --> Unsupported Function
*/
	virtual int GetCPUParameters(CPUParameters& stInfo) = 0;
	
};

#endif