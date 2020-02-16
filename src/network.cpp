#include <winsock2.h>
#include <windows.h>
#include <iphlpapi.h>
#include <cstdio>

#include "network.hpp"
#include "pdh.hpp"

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "iphlpapi.lib")

using namespace Network;

static void getPrimaryAdapter(DWORD *adapterIndex, WCHAR *adapterName)
{
    static DWORD dst = 1 << 24 | 1 << 16 | 1 << 8 | 1;
    DWORD ifIndex;
    GetBestInterface(dst, &ifIndex);
    *adapterIndex = ifIndex;

    ULONG outBufLen = 15000;
    PIP_ADAPTER_ADDRESSES pAddresses = NULL;
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;
    int i = 0;
    do
    {
        pAddresses = (IP_ADAPTER_ADDRESSES *)MALLOC(outBufLen);
        dwRetVal = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses, &outBufLen);
        if (dwRetVal == ERROR_BUFFER_OVERFLOW)
        {
            FREE(pAddresses);
            pAddresses = NULL;
        }
        else
        {
            break;
        }
        i++;
    } while ((dwRetVal == ERROR_BUFFER_OVERFLOW) && (i < 3));

    PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;
    while (pCurrAddresses)
    {
        if (pCurrAddresses->IfIndex == ifIndex)
        {
            wcscpy(adapterName, pCurrAddresses->Description);
            break;
        }
        pCurrAddresses = pCurrAddresses->Next;
    }

    if (pAddresses)
    {
        FREE(pAddresses);
    }
}

const Info &Network::InfoGetter::GetInfo(UINT16 sampleTime)
{
    DWORD curAdapterIndex = 0;
    getPrimaryAdapter(&curAdapterIndex, curAdapterName);
    if (0 != curAdapterIndex && curAdapterIndex == oldAdapterIndex)
    {
        goto sample;
    }
    oldAdapterIndex = curAdapterIndex;
    swprintf(queryUpStr,
             sizeof(queryUpStr) / sizeof(WCHAR),
             L"\\Network Adapter(%ws)\\Bytes Sent/sec",
             curAdapterName);
    swprintf(queryDownStr,
             sizeof(queryDownStr) / sizeof(WCHAR),
             L"\\Network Adapter(%ws)\\Bytes Received/sec",
             curAdapterName);
    if (upCounter != NULL)
    {
        pdhQ.RemoveCounter(upCounter);
    }
    if (downCounter != NULL)
    {
        pdhQ.RemoveCounter(downCounter);
    }
    upCounter = pdhQ.AddCounter(queryUpStr);
    downCounter = pdhQ.AddCounter(queryDownStr);

sample:
    pdhQ.Sample(sampleTime);
    pdhQ.GetValue(upCounter, PDH::LONG, &value);
    info.upSpeed = value.longValue / 1024;
    pdhQ.GetValue(downCounter, PDH::LONG, &value);
    info.downSpeed = value.longValue / 1024;
    return info;
}

void Network::InfoGetter::PrintInfo()
{
    wprintf(L"Network: %s\n", curAdapterName);
    printf("up: %d KB/s\tdown: %d KB/s\n", info.upSpeed, info.downSpeed);
}
