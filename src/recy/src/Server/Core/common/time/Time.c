#include <time.h>
#include <stdio.h>
#include "Time.h"
#include "Typedef.h"

static struct timespec ts_start = { 0, 0 };
static struct timespec ts_end = { 0, 0 };

void syncTime( const char *msg )
{
    UNUSED( msg );
    assert_m( 0 == clock_gettime(CLOCK_MONOTONIC, &ts_start), "clock_gettime" );
}

void checkTime( const char *msg )
{
    assert_m( 0 == clock_gettime(CLOCK_MONOTONIC, &ts_end), "clock_gettime" );
    printf("%s cost time: %ld\n", msg, ( ts_end.tv_sec - ts_start.tv_sec ) * 1000 + ( ts_end.tv_nsec - ts_start.tv_nsec ) / 1000000L );
    ts_start.tv_sec     = ts_end.tv_sec;
    ts_start.tv_nsec    = ts_end.tv_nsec;
}