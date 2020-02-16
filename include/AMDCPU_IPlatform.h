//===----------------------------------------------------------------------===
//=== Copyright (c) 2020 Advanced Micro Devices, Inc.  All rights reserved.
//
//            Developed by: Advanced Micro Devices, Inc.

#ifndef _AMDCPU_IPLATFORM_H_
#define _AMDCPU_IPLATFORM_H_

/**
* @file IPlatform.h 
* @brief Platform header file
*/

#include <set>
#include "AMDCPU_DeviceType.h"

#ifdef PLATFORM_EXPORTS
#define PLATFORM_API __declspec(dllexport)
#else
#define PLATFORM_API __declspec(dllimport)
#endif

class IDeviceManager;

typedef std::set<AOD_DEVICE_TYPE> CREATE_DEVICE_SET;

class IPlatform
{
public:

	/**Initialize the platform class. This function must be called before all other functions of this interface.
	\param	pszBoardVendor		[in]	Specify the vendor name or leave it as NULL
	\param	pCreateDeviceSet	[in]	Specify the device list that you'd like to use. If it is NULL, all available 
										devices will be created.
	\return True if succeeded; otherwise false.*/
	virtual bool Init(const char* pszBoardVendor = NULL,
		bool bUseCPUOnly = false) = 0;

	/**Uninitialize the platform class. After this function is called, all platform resources are released.*/
	virtual bool UnInit(void) = 0;

	/**Return the reference to the IDeviceManager interface*/
	virtual IDeviceManager& GetIDeviceManager(void) = 0;

};

/**
* @class IPlatform
* @brief The platform class.
* @details This class supplies all platform-related features.
*/

PLATFORM_API IPlatform& GetPlatform(void);

#endif	//_IPLATFORM_H_
