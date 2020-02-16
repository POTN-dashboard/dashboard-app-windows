#pragma once

#include <windows.h>

//Error codes
#define DL_OK (DWORD)0
#define DL_NOT_CREATE (DWORD)1
#define DL_NOT_START (DWORD)2
#define DL_NOT_INIT (DWORD)3
#define DL_ERROR_SCMANAGER (DWORD)4
#define DL_ERROR_CREATE (DWORD)5
#define DL_ERROR_START (DWORD)6
#define DL_ERROR_OPEN (DWORD)7
#define DL_ERROR_STOP (DWORD)8
#define DL_ERROR_UNLOAD (DWORD)9

class DriverLoader
{
public:
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
