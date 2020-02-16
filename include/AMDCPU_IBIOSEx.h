//===----------------------------------------------------------------------===//
//====  Copyright (c) 2020 Advanced Micro Devices, Inc.  All rights reserved.
//
//               Developed by: Advanced Micro Devices, Inc.

/**
* @file IBIOSEx.h
* @brief Processor header file having extended functionality for AMD RYZEN processor and above
*/

#ifndef _AMDCPU_IBIOSEx_H_
#define _AMDCPU_IBIOSEx_H_

#include "AMDCPU_IDevice.h"

class IBIOSEx : public IDevice
{
public:
/**
* Gets the current memory VDDIO voltage supply value in millivolts.
* @param uVDDioMV [out] Address of unsigned short variable which will hold current memory VDDIO voltage supply value
* @return Status of function call
*	- 0 --> Function call is successful
*	- 2 --> Method is not implemented by the BIOS
*	- -1 --> Failure in execution of the command in BIOS
*/
	virtual int GetMemVDDIO(unsigned short& uVDDioMV) = 0;

/**
* Gets the current memory clock frequency in MHz.
* @param uMemClock [out] Address of unsigned short variable which will hold current memory clock frequency
* @return Status of function call
*	- 0 --> Function call is successful
*	- 2 --> Method is not implemented by the BIOS
*	- -1 --> Failure in execution of the command in BIOS
*/
	virtual int GetCurrentMemClock(unsigned short& uMemClock) = 0;

/**
* Gets the current memory CAS latency.
* @param uTcl [out] Address of unsigned character variable which will hold CAS latency value
* @return Status of function call
*	- 0 --> Function call is successful
*	- 2 --> Method is not implemented by the BIOS
*	- -1 --> Failure in execution of the command in BIOS
*/
	virtual int GetMemCtrlTcl(unsigned char& uTcl) = 0;

/**
* Gets the current read row address to column address delay.
* @param uTrcdrd [out] Address of unsigned character variable which will hold read row address to column address delay.
* @return Status of function call
*	- 0 --> Function call is successful
*	- 2 --> Method is not implemented by the BIOS
*	- -1 --> Failure in execution of the command in BIOS
*/
	virtual int GetMemCtrlTrcdrd(unsigned char& uTrcdrd) = 0;

/**
* Gets the row active time.
* @param uTras [out] Address of unsigned character variable which will hold row active time.
* @return Status of function call
*	- 0 --> Function call is successful
*	- 2 --> Method is not implemented by the BIOS
*	- -1 --> Failure in execution of the command in BIOS
*/
	virtual int GetMemCtrlTras(unsigned char& uTras) = 0;

/**
* Gets the row pre-charge delay.
* @param uTrp [out] Address of unsigned character variable which will hold row precharge delay.
* @return Status of function call
*	- 0 --> Function call is successful
*	- 2 --> Method is not implemented by the BIOS
*	- -1 --> Failure in execution of the command in BIOS
*/
	virtual int GetMemCtrlTrp(unsigned char& uTrp) = 0;

/**
* Gets the BIOS Version.
* @param void
* @return Returns Bios Version.
*/
	virtual const wchar_t* GetVersion(void) = 0;

/**
* Gets the BIOS Vendor.
* @param void
* @return Returns the name of the BIOS Vendor.
*/
	virtual const wchar_t* GetVendor(void) = 0;

/**
* Gets the BIOS Date.
* @param void
* @return Returns the Date of the BIOS.
*/
	virtual const wchar_t* GetDate(void) = 0;
};

#endif