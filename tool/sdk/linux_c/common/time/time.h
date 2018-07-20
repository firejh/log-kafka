#ifndef COMMON_TIME_H
#define COMMON_TIME_H

#include <time.h>
#include <sys/time.h>
#include <string>

namespace common{

static std::string get_time_us()
{
    char buf[64] = {0};
    struct timeval tv;
    struct tm tm_time;
    gettimeofday(&tv, NULL);

    localtime_r(&tv.tv_sec, &tm_time);
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
             tm_time.tm_year + 1900,
             tm_time.tm_mon + 1,
             tm_time.tm_mday,
             tm_time.tm_hour,
             tm_time.tm_min,
             tm_time.tm_sec,
             tv.tv_usec);

    return buf;

}

static uint64_t get_time_ms_n()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

}
#endif
