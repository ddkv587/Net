#ifndef __PROCESSERHPP__
#define __PROCESSERHPP__

#include "common.h"

#include <sys/types.h>
#include <sys/event.h>
#include <sys/time.h>
#include <thread>

class CProcesser
{
public:
	CProcesser();
	virtual ~CProcesser();

	int 		addEvent(int fd, int mask = NET_READABLE);
	void		delEvent(int fd, int mask = NET_READABLE);

	void 		run();
	void 		mainLoop();

	CProcesser(CProcesser&) = delete;
	CProcesser(const CProcesser&) = delete;
private:

private:
	int m_kqfd;
	struct kevent m_events[SYSTEM_LIMIT_MAX_EPOLL_EVENTS];
	std::thread m_thread;
};
#endif
