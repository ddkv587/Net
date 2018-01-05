#ifndef __COMMONHH__
#define __COMMONHH__

#include <stdio.h>
#include <errno.h>
#include <assert.h>

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

#define handle_error_errno(en, msg) \
	do { errno = en, perror(msg); exit(EXIT_FAILURE); } while(0)

#define SYSTEM_LIMIT_MAX_EPOLL_EVENTS 1000

extern int errno;
static int s_iStop = 0;

void chomp(char *buff, int size)
{
	if ( buff[size - 2] == 0x0d )
	{
		buff[size - 2] = 0;
		buff[size - 1] = 0;
	}
}

#endif
