#ifndef __COMMONHH__
#define __COMMONHH__

#include <vector>

#ifndef DEF_ASSERT
#include <assert.h>
#endif

#include "log/NET_Log.hpp"

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

#define handle_error_errno(en, msg) \
	do { errno = en, perror(msg); exit(EXIT_FAILURE); } while(0)

#define CHECK_R(X, R) \
	do { if (!(X)) return (R); } while(0)

#define STRING_AUX(X) #X

#define DEFINE_ONCE(name, value) \
	STRING_AUX(ifndef) name \
	STRING_AUX(define) name value \
	STRING_AUX(endif)			//error

#define SYSTEM_VERSION					0.1 
#define SYSTEM_MAX_EVENTS 				1024
#define SYSTEM_SOCKET_PORT				8000

#define POLL_TIMEOUT_SEC 5
#define POLL_TIMEOUT_USEC 0

#define NET_NONE 		0x00
#define NET_READABLE 	0x01
#define NET_WRITABLE 	0x02

extern "C" {
/*
	void chomp(char *buff, int size)
	{
		if ( buff[size - 2] == 0x0d )
		{
			buff[size - 2] = 0;
			buff[size - 1] = 0;
		}
	}
*/
}

#endif
