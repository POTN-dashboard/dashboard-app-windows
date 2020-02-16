//===----------------------------------------------------------------------===//
//====  Copyright (c) 2020 Advanced Micro Devices, Inc.  All rights reserved.
//
//               Developed by: Advanced Micro Devices, Inc.

/**
* @file IDevice.h
* @brief Device header file
*/

#ifndef _AMDCPU_IDEVICE_H_
#define _AMDCPU_IDEVICE_H_

#include "AMDCPU_DeviceType.h"

//AOD Status code

enum AOD_STATUS_CODE
{
	bFailure = -1,
	bSuccess = 0,
	bInvalidInput = 1,
	bBiosFuncNotImpemented = 2,
	bCoreParkingDisabled = 3,
	bUnsupportedFunction
};

typedef union OCMode
{
	unsigned int uOCMode;
	struct
	{
		unsigned uAuto : 1;				///< Auto or Default Mode
		unsigned uManual : 1;			///< Manual Overclocking Mode
		unsigned uPBOMode : 1;			///< PBO Mode
		unsigned uAutoOverclocking : 1;	///< AutoOverclocking Mode
		unsigned Reserved : 28;
	};

} OCMode;

/// The cache information structure.
struct CACHE_INFO
{
	double fSize;					///< Cache size (KB)
	double fAssociativity;			///< Cache associativity
	double fLines;					///< Cache lines per tag
	double fLineSize;				///< Cache line size (bytes)
};

typedef struct EffectiveFreqData {
	unsigned int uLength;			///< Length of the Array  
	double *dFreq;					///< Address of the Array of Effective Frequency in Mhz
	double *dState;					///< Address of the Array of C0 State Residency in %
}EffectiveFreqData;

typedef struct CPUParameters
{
	OCMode eMode;					///< OCMode Structure
	EffectiveFreqData stFreqData;	///< EffectiveFreqData Structure
	double dPeakCoreVoltage;		///< Peak Core Voltage
	double dSocVoltage;				///< Current SOC Voltage
	double dTemperature;			///< Current Temperature(C)
	double dAvgCoreVoltage;			///< Average Core Voltage of that Sample period
	double dPeakSpeed;				///< Peak Speed of all the Active cores
	float fPPTLimit;				///< PPT Current Limit(W)
	float fPPTValue;				///< PPT Current value(W)
	float fTDCLimit_VDD;			///< TDC(VDD) Current Limit(A)
	float fTDCValue_VDD;			///< TDC(VDD) Current Value(A)
	float fEDCLimit_VDD;			///< EDC(VDD) Current limit(A)
	float fEDCValue_VDD;			///< EDC(VDD) Current Value(A)
	float fcHTCLimit;				///< cHTC Current Limit(celsius)
	float fFCLKP0Freq;				///< FCLK P0 Frequency
	float fCCLK_Fmax;				///< CCLK Fmax frequency(MHz)
	float fTDCLimit_SOC;			///< TDC(SOC) limit in ampere(A)
	float fTDCValue_SOC;			///< TDC(SOC) current value  in ampere(A)
	float fEDCLimit_SOC;			///< EDC(SOC) limit  in ampere(A)
	float fEDCValue_SOC;			///< EDC(SOC) Avg value  in ampere(A)
	float fVDDCR_VDD_Power;			///< VDDCR_VDD Power in watts(W)
	float fVDDCR_SOC_Power;			///< VDDCR_SOC Power in watts(W)
}CPUParameters;
#ifndef DOXYGEN_SHOULD_SKIP_THIS
class IDevice
{
public:
	/**
	* Initialize the device. This function must be called before other functions in this class and its subclasses.
	* @param uIndex	[in]	Specifies the index of the instance to initialize.
	* @return True if successful; otherwise false.
	*/
	virtual bool Init(unsigned long uIndex) = 0;

	/**
	* Uninitialize the device.
	* @param void
	* @return True if successful; otherwise false.
	*/
	virtual bool UnInit(void) = 0;

	/**
	* Gets the Processor Name String Identifier as obtained from CPUID instruction.
	* @param void
	* @return Returns Processor Name String Identifier.
	*/
	virtual const wchar_t* GetName(void) = 0;

	/**
	* Gets the device description which includes BaseFamily, ExtendedFamily, BaseModel, ExtendedModel, Stepping detail as obtained from CPUID instruction.
	* @param void
	* @return Returns device description string.
	*/
	virtual const wchar_t* GetDescription(void) = 0;

	/**
	* Gets the device vendor name as obtained from CPUID instruction.
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

};
#endif
#endif