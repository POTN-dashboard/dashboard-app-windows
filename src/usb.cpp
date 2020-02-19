#include <cstdio>
#include <stdexcept>
#include <string>
#include <comdef.h>

#include "usb.hpp"

#pragma comment(lib, "hidapi.lib")

using namespace USB;

std::runtime_error USB::Connector::error(const char *msg)
{
    std::string errorStr = "[USB] ";
    errorStr += msg;
    if (NULL != dev)
    {
        _bstr_t error(hid_error(dev));
        const char *errorMsg = error;
        errorStr += ": ";
        errorStr += errorMsg;
    }
    return std::runtime_error(errorStr);
}

USB::Connector::Connector() : dev(NULL)
{
    int res = hid_init();
    if (0 != res)
    {
        throw error("Init hidapi fail");
    }
}

USB::Connector::~Connector()
{
    if (NULL != dev)
    {
        hid_close(dev);
    }
    hid_exit();
    dev = NULL;
}

bool USB::Connector::Connect()
{
    hid_device_info *devs = hid_enumerate(VID, PID);
    hid_device_info *curDev = devs;
    char path[128];
    path[0] = 0;
    while (NULL != curDev)
    {
        if (INTERFACE_INDEX == curDev->interface_number)
        {
            strcpy(path, curDev->path);
            break;
        }
        curDev = curDev->next;
    }
    if (0 == path[0])
    {
        return false;
    }
    dev = hid_open_path(path);
    if (NULL == dev)
    {
        throw error("Open device fail");
    }
    return true;
}

Error USB::Connector::Read(BYTE *buf, int bufLen, int *actualLen)
{
    int res = hid_read_timeout(dev, buf, bufLen, TIMEOUT);
    if (0 == res)
    {
        return Error::TIMEOUT;
    }
    if (0 < res)
    {
        *actualLen = res;
        // printf("[USB] Read %d bytes\n", res);
        return Error::OK;
    }
    wprintf(L"[USB] Read fail: %ls\n", hid_error(dev));
    return Error::DISCONNECTED;
}

Error USB::Connector::Write(BYTE *buf, int len)
{
    BYTE writeBuf[MAX_PACK_SIZE + 1];
    writeBuf[0] = 0;
    memcpy(writeBuf + 1, buf, len);
    int res = hid_write(dev, writeBuf, len + 1);
    if (-1 == res)
    {
        wprintf(L"[USB] Write fail: %ls\n", hid_error(dev));
        return Error::DISCONNECTED;
    }
    // wprintf(L"[USB] Write %d bytes\n", res);
    return Error::OK;
}