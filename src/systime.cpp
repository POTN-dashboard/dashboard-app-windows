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

void SystemTime::Time::Serialize(BYTE *buf)
{
    // year (Big Endian)
    buf[0] = (BYTE)(year >> 8);
    buf[1] = (BYTE)year;

    // month
    buf[2] = month;
    // day of week
    buf[3] = dayOfWeek;
    // day
    buf[4] = day;
    // hour
    buf[5] = hour;
    // minute
    buf[6] = minute;
}

void SystemTime::Time::Print()
{
    static char *week[] = {
        "星期零（呵呵",
        "星期一",
        "星期二",
        "星期三",
        "星期四",
        "星期五",
        "星期六",
        "星期日",
    };
    printf("%d-%d-%d %s %d:%d\n", year, month, day, week[dayOfWeek], hour, minute);
}