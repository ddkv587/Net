#ifndef __TIMEH__
#define __TIMEH__

extern void syncTime( const char *msg );
extern void checkTime( const char *msg );

#ifdef __DEBUG__
#define TIME_SYNC( X )      syncTime( X )
#define TIME_CHECK( X )     checkTime( X )
#else
#define TIME_SYNC( X ) 
#define TIME_CHECK( X )
#endif

#endif