#ifndef __PROCESSERHPP__
#define __PROCESSERHPP__

#include "common.h"

#ifdef KQUEUE
#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#endif

class CProcesser
{
public:
	CProcesser();
	virtual ~CProcesser();

	void 		setThreadID(pthread_t id) { m_thread.thread_id = id; }
	pthread_t 	getThreadID() { return m_thread.thread_id; }

	int 		addEvent(int fd, int mask = NET_READABLE);
	void		delEvent(int fd);

	void 		run();

private:
	void* mainLoop(void*);

private:
	int m_kqfd;
	struct kevent m_events[SYSTEM_LIMIT_MAX_EPOLL_EVENTS];
	thread_info m_thread;
};
#endif
