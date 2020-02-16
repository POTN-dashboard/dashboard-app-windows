//===----------------------------------------------------------------------===//
//====  Copyright (c) 2020 Advanced Micro Devices, Inc.  All rights reserved.
//
//               Developed by: Advanced Micro Devices, Inc.

/*!
* @file DeviceType.h
* @brief Device type header file
*/

#ifndef _AMDCPU_DEVICE_TYPE_H_
#define _AMDCPU_DEVICE_TYPE_H_

#ifdef DEVICE_EXPORTS
#define DEVICE_API __declspec(dllexport)
#else
#define DEVICE_API 
#endif

/*!
* @details
*	Each device type has a corresponding interface class.
*	When creating a new device type, make sure the type number is larger than the value of dtPluginDevice.
*/

enum AOD_DEVICE_TYPE
{
	dtInvalid = -1,
	/// ICPU
	dtCPU,
	/// IBIOS
	dtBIOS,	
};

#endif	//_DEVICE_TYPE_H_
