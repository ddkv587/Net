#include "MultiEpool.hpp"

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

	int CMultiEpoll::addFileEvent(int, int)
	{
		;
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
