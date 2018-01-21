#ifndef __PROCESSERHPP__
#define __PROCESSERHPP__

#include "common.h"

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>

class CProcesser
{
public:
	CProcesser();
	virtual ~CProcesser();

	pthread_t 	getThreadID() { return m_thread.thread_id; }

	int 		addEvent(int fd, int mask = NET_READABLE);
	void		delEvent(int fd, int mask = NET_READABLE);

	void 		run();
	void* mainLoop(void*);

private:

private:
	int m_kqfd;
	struct kevent m_events[SYSTEM_LIMIT_MAX_EPOLL_EVENTS];
	thread_info m_thread;
};
#endif
