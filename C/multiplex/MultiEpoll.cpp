#include "Multiplex.hpp"

namespace NET
{
	CMultiEpoll::CMultiEpoll()
		: m_epfd(-1)
		, m_pEvents(NULL)
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

		ee.events = 0;
		if ( mask & NET_READABLE ) ee.events |= EPOLLIN;
		if ( mask & NET_WRITABLE ) ee.events |= EPOLLOUT;

		ee.data.fd = fd;

		CHECK_R( -1 != epoll_ctl(), -1 );

		return 0;
	}

	void CMultiEpoll::delFileEvent(int, int)
	{
		;
	}

	int CMultiEpoll::eventLoop(void*)
	{
		;
	}
}
