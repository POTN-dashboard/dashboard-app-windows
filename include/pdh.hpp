#pragma once

#include <pdh.h>

namespace PDH
{

typedef PDH_FMT_COUNTERVALUE VALUE;
typedef PDH_HCOUNTER COUNTER;

const DWORD LONG = PDH_FMT_LONG;
const DWORD DOUBLE = PDH_FMT_DOUBLE;
const DWORD LARGE = PDH_FMT_LARGE;
const DWORD NOCAP100 = PDH_FMT_NOCAP100;

class Query
{
public:
    Query();
    ~Query();

    COUNTER AddCounter(LPCTSTR queryStr);
    void RemoveCounter(COUNTER counter);
    void Sample(UINT16 time);
    void GetValue(COUNTER counter, DWORD valueType, VALUE *value);

private:
    HQUERY query;
    int nextCounterIndex;
};

}