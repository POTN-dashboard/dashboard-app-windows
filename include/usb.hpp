#pragma once

#include <windows.h>

#include "hidapi.h"

namespace USB
{

// Connection error codes
enum Error
{
    OK,
    TIMEOUT,
    DISCONNECTED,
};

class Connector
{
public:
    static const UINT16 VID = 0x0483;
    static const UINT16 PID = 0x5750;
    static const int INTERFACE_INDEX = 0;
    // time in ms.
    static const int TIMEOUT = 1000;
    static const int MAX_PACK_SIZE = 64;

    static const UINT8 READY_PACK = 0x11;
    static const UINT8 CPU_GPU_PACK = 0x22;
    static const UINT8 ACK_PACK = 0x33;
    static const UINT8 DATA_PACK = 0x44;

public:
    Connector();
    ~Connector();

    bool Connect();
    Error Read(BYTE *buf, int bufLen, int *actualLen);
    Error Write(BYTE *buf, int len);

private:
    hid_device *dev;

    std::runtime_error error(const char *msg);
};

} // namespace USB
