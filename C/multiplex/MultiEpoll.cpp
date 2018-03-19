#include "NET_Multiplex.hpp"

namespace NET
{
	CMultiEpoll::CMultiEpoll()
		: m_epfd(-1)
		, m_events(NULL)
	{
		m_epfd = epoll_create(1024);
		CHECK( -1 != m_epfd );

		m_eType = EMT_EPOLL;
	}

	CMultiEpoll::~CMultiEpoll()
	{
		;
	}

	int CMultiEpoll::addFileEvent(int fd, int mask)
	{
		struct epoll_event ee = {0};

		int op = m_eventLoop->event[fd].mask == NET_NONE ? 
					EPOLL_CTL_ADD : EPOLL_CTL_MOD;

		ee.events = 0;
		mask |= m_eventLoop->event[fd].mask;
		if ( mask & NET_READABLE ) ee.events |= EPOLLIN;
		if ( mask & NET_WRITABLE ) ee.events |= EPOLLOUT;

		ee.data.fd = fd;

		return epoll_ctl(m_epfd, op, fd, &ee);
	}

	void CMultiEpoll::delFileEvent(int fd, int mask)
	{
		struct epoll_event ee = {0};

		ee.events = 0;
		mask = m_eventLoop->event[fd].mask & (~mask);
		if ( mask & NET_READABLE ) ee.events |= EPOLLIN;
		if ( mask & NET_WRITABLE ) ee.events |= EPOLLOUT;

		ee.data.fd = fd;
		if ( mask != NET_NONE ) {
			epoll_ctl(m_epfd, EPOLL_CTL_MOD, fd, &ee);
		} else {
			epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, &ee);
		}
	}

	int CMultiEpoll::eventLoop(void* timeout)
	{
		int retval;

		struct timeval* tvl = timeout ? (struct timeval*)timeout : NULL;

		retval = epoll_wait( m_epfd, m_events, m_eventLoop->size, 
				tvl ? ( tvl->tv_sec * 1000 + tvl->tv_usec / 1000 ) : -1 );
	
		CHECK_R( retval != -1, -1 );

		for ( int index = 0; index < retval; ++index ) {
			int mask = 0;
			struct epoll_event* e = m_events + index;

			if ( e->events & EPOLLIN ) mask |= NET_READABLE;
			if ( e->events & EPOLLOUT ) mask |= NET_WRITABLE;
			if ( e->events & EPOLLERR ) mask |= NET_WRITABLE;
			if ( e->events & EPOLLHUP ) mask |= NET_WRITABLE;

			m_eventLoop->fired[index].fd = e->data.fd;
			m_eventLoop->fired[index].mask = mask;
		}
		return retval;
	}
}
