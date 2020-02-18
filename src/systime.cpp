#include <cstdio>

#include "systime.hpp"

using namespace SystemTime;

SystemTime::Time::Time()
{
    Update();
}

void SystemTime::Time::Update()
{
    GetLocalTime(&time);
    year = time.wYear;
    month = time.wMonth;
    day = time.wDay;
    dayOfWeek = (0 == time.wDayOfWeek) ? 7 : time.wDayOfWeek;
    hour = time.wHour;
    minute = time.wMinute;
}

void SystemTime::Time::Print()
{
    static char *week[] = {
        "error :)",
        "Mon.",
        "Tues.",
        "Wed.",
        "Thur.",
        "Fri.",
        "Sat.",
        "Sun.",
    };
    printf("%d-%d-%d %s %d:%d\n", year, month, day, week[dayOfWeek], hour, minute);
}