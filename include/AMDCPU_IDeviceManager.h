//===----------------------------------------------------------------------===//
//====  Copyright (c) 2020 Advanced Micro Devices, Inc.  All rights reserved.
//
//               Developed by: Advanced Micro Devices, Inc.

/**
* @file IDeviceManager.h
* @brief Device manager header file
*/

#ifndef _AMDCPU_IDEVICE_MANAGER_H_
#define _AMDCPU_IDEVICE_MANAGER_H_

#include "AMDCPU_IDevice.h"
#include <set>

#define CREATE_DEVICE_FUNC_NAME		"CreateDevice"
#define GET_DEVICE_CLASS_COUNT_FUNC_NAME	"GetDeviceClassCount"
#define GET_PROVIDER_FUNC_NAME		"GetProvider"

typedef unsigned long(*GET_DEVICE_CLASS_COUNT_FUNC)(void);
typedef IDevice* (*CREATE_DEVICE_FUNC)(unsigned long);
typedef const char* (*GET_PROVIDER)(void);

typedef std::set<AOD_DEVICE_TYPE> CREATE_DEVICE_SET;

class IDeviceManager
{
public:
	/**Initialize the device manager
	\param	pszBoardVendor		[in]	Specifies the vendor of motherboard.
	\param	pCreateDeviceSet	[in]	Specifies device set which allow device manager to create.
	\return	True if successful; otherwise false.
	*/
	virtual bool Init(const char* pszBoardVendor,
		bool bUseCPUOnly = false) = 0;

	/**Uninitialize the device manager
	\return	True if successful; otherwise false.
	*/
	virtual bool UnInit(void) = 0;

	/**Return the device instance by device type and index.
	\param	eDeviceType	[in]	Specifies the type of device.
	\param	uIndex		[in]	Specifies the index of the device instance, starts from zero.
	\return	Device instance if successful; otherwise NULL.
	*/
	virtual IDevice* GetDevice(AOD_DEVICE_TYPE eDeviceType, unsigned long uIndex) = 0;

	/**Return the device instance by enumeration index.
	\param	uIndex		[in]	Specifies the index in the device list, starts from zero.
	\return	Device instance if successful; otherwise NULL.
	*/
	virtual IDevice* GetDevice(unsigned long uIndex) = 0;

	/**Return the device instance count by device type.
	\param eDeviceType	[in]	Specifies the device type.
	\return Device instance count if successful; otherwise 0.*/
	virtual unsigned long GetDeviceCount(AOD_DEVICE_TYPE eDeviceType) = 0;

	/**Return the device instance count by device name.
	\param pszClassName	[in]	Specifies the device name.
	\return Device instance count if successful; otherwise 0.*/
	virtual unsigned long GetDeviceCount(const wchar_t* pszClassName) = 0;

	/**Return the number of all device types.
	\return Device count of all available devices if successful; otherwise 0.*/
	virtual unsigned long GetTotalDeviceCount() = 0;

};

/**
* @class IDeviceManager
* @brief The device manager class.
*/
DEVICE_API IDeviceManager& GetDeviceManager(void);

#endif	//_IDEVICE_MANAGER_H_