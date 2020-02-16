#pragma once

#include <windows.h>

class DriverLoader
{
public:
	// Error Codes.
	static const DWORD OK = 0;
	static const DWORD NOT_CREATE = 1;
	static const DWORD NOT_START = 2;
	static const DWORD NOT_INIT = 3;
	static const DWORD ERROR_SCMANAGER = 4;
	static const DWORD ERROR_CREATE = 5;
	static const DWORD ERROR_START = 6;
	static const DWORD ERROR_OPEN = 7;
	static const DWORD ERROR_STOP = 8;
	static const DWORD ERROR_UNLOAD = 9;

	//Constructor
	DriverLoader(void);
	DriverLoader(LPCWSTR fileName, LPCWSTR _lpServiceName, LPCWSTR _lpDisplayName, DWORD _dwStartType);
	//Destructor
	~DriverLoader(void);

	//init function
	DWORD InitSvc(LPCWSTR fileName, LPCWSTR _lpServiceName, LPCWSTR _lpDisplayName, DWORD _dwStartType);
	DWORD CreateSvc(void); //create service
	DWORD UnloadSvc(void); //unload service
	DWORD StartSvc(void);  //start service
	DWORD StopSvc(void);   //stop service

	BOOL IsInit(void);	//check if service is init
	BOOL IsLoaded(void);  //check if service is loaded
	BOOL IsStarted(void); //check if service is started

private:
	LPCWSTR lpFilePath;	//driver file path
	LPCWSTR lpServiceName; //service name
	LPCWSTR lpDisplayName; //dos service name

	DWORD dwStartType; //start type

	SC_HANDLE hService; //service's handle

	// Status variables
	BOOL init;
	BOOL loaded;
	BOOL started;
};
