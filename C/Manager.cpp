#include <sys/epoll.h>

#include "common.h"
#include "Manager.hpp"

static int fd_epoll;

void initManager()
{
	fd_epoll = epoll_create(SYSTEM_LIMIT_MAX_EPOLL_EVENTS + 1);

	if ( fd_epoll == -1 ) {
		handle_error("epoll_create");
	}
}

void destroyManager()
{
	if ( fd_epoll > 0 ) {
		
	}

}

void* handover(void* arg)
{
	while(1)
	{
				
	}
}
