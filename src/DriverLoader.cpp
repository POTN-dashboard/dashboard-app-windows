#include <windows.h>
#include <cstdio>

#include "DriverLoader.hpp"

#pragma comment(lib, "Advapi32.lib")

static void getDriverPath(WCHAR *driverPath, WCHAR *driverFileName)
{
	WCHAR fullpath[MAX_PATH];
	if (0 == GetModuleFileNameW(0, fullpath, MAX_PATH))
	{
		return;
	}
	WCHAR drive[8];
	WCHAR path[MAX_PATH];
	_wsplitpath(fullpath, drive, path, NULL, NULL);
	driverPath[0] = 0;
	wcscat(driverPath, drive);
	wcscat(driverPath, path);
	wcscat(driverPath, driverFileName);
}

DriverLoader::DriverLoader(void)
{
	lpFilePath = NULL;
	lpServiceName = NULL;
	lpDisplayName = NULL;

	dwStartType = 0;

	hService = NULL;

	init = false;
	loaded = false;
	started = false;
}

DriverLoader::DriverLoader(LPCWSTR fileName, LPCWSTR _lpServiceName, LPCWSTR _lpDisplayName, DWORD _dwStartType)
{
	WCHAR *driverPath = new WCHAR[MAX_PATH];
	getDriverPath(driverPath, (WCHAR *)fileName);
	lpFilePath = driverPath;
	lpServiceName = _lpServiceName;
	lpDisplayName = _lpDisplayName;

	dwStartType = _dwStartType;

	hService = NULL;

	init = true;
	loaded = false;
	started = false;
}

DriverLoader::~DriverLoader(void)
{
	UnloadSvc();

	delete[] lpFilePath;
	lpFilePath = NULL;
	lpServiceName = NULL;
	lpDisplayName = NULL;

	dwStartType = 0;

	hService = NULL;

	init = false;
	loaded = false;
	started = false;
}

DWORD DriverLoader::InitSvc(LPCWSTR fileName, LPCWSTR _lpServiceName, LPCWSTR _lpDisplayName, DWORD _dwStartType)
{
	if (IsInit())
		return DriverLoader::OK;

	WCHAR *driverPath = new WCHAR[MAX_PATH];
	getDriverPath(driverPath, (WCHAR *)fileName);
	lpFilePath = driverPath;
	lpServiceName = _lpServiceName;
	lpDisplayName = _lpDisplayName;

	dwStartType = _dwStartType;

	hService = NULL;

	init = true;
	loaded = false;
	started = false;

	return DriverLoader::OK;
}

DWORD DriverLoader::CreateSvc(void)
{
	if (!IsInit())
		return DriverLoader::NOT_INIT;

	if (IsLoaded())
		return DriverLoader::OK;

	SC_HANDLE hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

	if (hSCManager == NULL)
		return DriverLoader::ERROR_SCMANAGER;

	hService = CreateServiceW(hSCManager, lpServiceName, lpDisplayName,
							  SERVICE_ALL_ACCESS,
							  SERVICE_KERNEL_DRIVER,
							  dwStartType,
							  SERVICE_ERROR_NORMAL,
							  lpFilePath,
							  NULL,
							  NULL,
							  NULL,
							  NULL,
							  NULL);

	if (hService == NULL)
	{
		DWORD dwLastError = GetLastError();
		if (dwLastError == ERROR_SERVICE_EXISTS)
		{
			hService = OpenServiceW(hSCManager, lpServiceName, SERVICE_ALL_ACCESS);
		}
		else if (dwLastError == ERROR_SERVICE_MARKED_FOR_DELETE)
		{
			hService = OpenServiceW(hSCManager, lpServiceName, SERVICE_ALL_ACCESS);
			SERVICE_STATUS ServiceStatus;
			ControlService(hService, SERVICE_CONTROL_STOP, &ServiceStatus);
			hService = CreateServiceW(hSCManager,
									  lpServiceName, lpServiceName, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
									  SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, lpFilePath,
									  NULL, NULL, NULL, NULL, NULL);
		}

		if (hService == NULL)
		{
			CloseServiceHandle(hSCManager);
			wprintf(L"[DriverLoader] Driver path: %s\n", lpFilePath);
			return DriverLoader::ERROR_CREATE;
		}
	}

	loaded = true;
	CloseServiceHandle(hSCManager);

	return DriverLoader::OK;
}

DWORD DriverLoader::StartSvc(void)
{
	if (!IsLoaded())
		return DriverLoader::NOT_CREATE;

	if (IsStarted())
		return DriverLoader::OK;

	SC_HANDLE hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

	if (hSCManager == NULL)
		return DriverLoader::ERROR_SCMANAGER;

	hService = OpenServiceW(hSCManager, lpServiceName, SERVICE_ALL_ACCESS);

	if (hService == NULL)
	{
		CloseServiceHandle(hSCManager);
		return DriverLoader::ERROR_OPEN;
	}

	DWORD dwLastError;
	if (StartServiceW(hService, 0, NULL) == NULL)
	{
		dwLastError = GetLastError();
		if (dwLastError == ERROR_SERVICE_ALREADY_RUNNING)
		{
			goto succeed;
		}

		if (dwLastError == ERROR_PATH_NOT_FOUND)
		{
			DeleteService(hService);
			CloseServiceHandle(hService);
			hService = CreateServiceW(hSCManager,
									  lpServiceName, lpServiceName, SERVICE_ALL_ACCESS, SERVICE_KERNEL_DRIVER,
									  SERVICE_DEMAND_START, SERVICE_ERROR_NORMAL, lpFilePath,
									  NULL, NULL, NULL, NULL, NULL);
			if (StartServiceW(hService, 0, NULL))
			{
				goto succeed;
			}
		}

		printf("[DriverLoader] Last Error: %d\n", dwLastError);

		CloseServiceHandle(hSCManager);
		CloseServiceHandle(hService);
		return DriverLoader::ERROR_START;
	}

succeed:
	CloseServiceHandle(hSCManager);
	started = true;

	return DriverLoader::OK;
}

DWORD DriverLoader::StopSvc(void)
{
	SERVICE_STATUS ss;

	if (!IsStarted())
		return DriverLoader::OK;

	SC_HANDLE hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

	if (hSCManager == NULL)
		return DriverLoader::ERROR_SCMANAGER;

	hService = OpenServiceW(hSCManager, lpServiceName, SERVICE_ALL_ACCESS);

	if (hService == NULL)
	{
		CloseServiceHandle(hSCManager);
		return DriverLoader::ERROR_OPEN;
	}

	if (ControlService(hService, SERVICE_CONTROL_STOP, &ss) == NULL)
	{
		CloseServiceHandle(hSCManager);
		CloseServiceHandle(hService);
		return DriverLoader::ERROR_STOP;
	}

	started = false;

	CloseServiceHandle(hSCManager);
	CloseServiceHandle(hService);
	return DriverLoader::OK;
}

DWORD DriverLoader::UnloadSvc(void)
{
	if (!IsLoaded())
		return DriverLoader::OK;

	if (IsStarted())
		if (StopSvc() != DriverLoader::OK)
			return DriverLoader::ERROR_UNLOAD;

	SC_HANDLE hSCManager = OpenSCManagerW(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

	if (hSCManager == NULL)
		return DriverLoader::ERROR_SCMANAGER;

	hService = OpenServiceW(hSCManager, lpServiceName, SERVICE_ALL_ACCESS);

	if (hService == NULL)
	{
		CloseServiceHandle(hSCManager);
		return DriverLoader::ERROR_OPEN;
	}

	DeleteService(hService);
	CloseServiceHandle(hSCManager);

	loaded = false;

	return DriverLoader::OK;
}

BOOL DriverLoader::IsInit(void)
{
	return init;
}

BOOL DriverLoader::IsLoaded(void)
{
	return loaded;
}

BOOL DriverLoader::IsStarted(void)
{
	return started;
}