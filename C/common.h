#ifndef __COMMONHH__
#define __COMMONHH__

#include <stdio.h>
#include <errno.h>
#include <assert.h>

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while(0)

#define handle_error_errno(en, msg) \
	do { errno = en, perror(msg); exit(EXIT_FAILURE); } while(0)

#define SYSTEM_VERSION					0.1 
#define SYSTEM_LIMIT_MAX_EPOLL_EVENTS 	1000
#define SYSTEM_SOCKET_PORT				8000

#define NET_NONE		0
#define NET_READABLE	1
#define NET_WRITABLE	2

#define POLL_TIMEOUT_SEC 5
#define POLL_TIMEOUT_USEC 0

struct thread_info{
//	pthread_t 	thread_id;
	int 		thread_num;
	char		*argv;
};

extern int errno;
static int s_iStop = 0;

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
