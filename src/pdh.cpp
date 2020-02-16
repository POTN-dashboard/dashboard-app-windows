#include <windows.h>
#include <pdh.h>

#include "pdh.hpp"

#pragma comment(lib, "pdh.lib")

using namespace PDH;

PDH::Query::Query()
{
    PdhOpenQuery(NULL, NULL, &query);
    nextCounterIndex = 0;
}

PDH::Query::~Query()
{
    PdhCloseQuery(query);
}

COUNTER PDH::Query::AddCounter(LPCTSTR queryStr)
{
    COUNTER counter;
    PdhAddCounter(query, queryStr, NULL, &counter);
    return counter;
}

void PDH::Query::RemoveCounter(COUNTER counter)
{
    PdhRemoveCounter(counter);
}

void PDH::Query::Sample(UINT16 time)
{
    PdhCollectQueryData(query);
    Sleep(time);
    PdhCollectQueryData(query);
}

void PDH::Query::GetValue(COUNTER counter, DWORD valueType, VALUE *value)
{
    PdhGetFormattedCounterValue(counter, valueType, NULL, value);
}
