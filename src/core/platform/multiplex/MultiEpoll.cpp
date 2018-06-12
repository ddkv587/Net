#include "NET_Core.hpp"

namespace NET
{
	CMultiEpoll::CMultiEpoll()
		: m_epfd(-1)
		, m_events(nullptr)
	{
		m_eType = EMT_EPOLL;
	}

	CMultiEpoll::~CMultiEpoll()
	{
		;
	}

	void CMultiEpoll::init()
	{
		m_epfd = epoll_create(1024);
		CHECK( -1 != m_epfd );

		LOG(INFO) << CLog::format("epoll init: %d\n", m_epfd);
	}

	void CMultiEpoll::destroy()
	{
        close(m_epfd);
        
        if ( NULL != m_events ) {
            delete m_events;
        }
	}
	
	INT CMultiEpoll::setSize(INT size)
	{
		if ( NULL != m_events ) {
			delete m_events;	
		}
		m_events = new struct epoll_event[size];

		return size;
	}

	INT CMultiEpoll::addFileEvent(INT fd, INT mask)
	{
		struct epoll_event ee = {0};

		INT op = m_eventLoop->lstFileEvent[fd].mask == NET_NONE ? EPOLL_CTL_ADD : EPOLL_CTL_MOD;

		ee.events = 0;
		mask |= m_eventLoop->lstFileEvent[fd].mask;
		if ( mask & NET_READABLE ) { 
			ee.events = EPOLLIN;
			if (m_bIsEdgeTrigger) ee.events |= EPOLLET;
			LOG(INFO) << CLog::format("%s epoll read\n", op == EPOLL_CTL_ADD ? "add" : "mod"); 
		}

		if ( mask & NET_WRITABLE ) { 
			ee.events = EPOLLOUT; 
			LOG(INFO) << CLog::format("%s epoll write\n", op == EPOLL_CTL_ADD ? "add" : "mod"); 
		}
		ee.data.fd = fd;

		return epoll_ctl(m_epfd, op, fd, &ee);
	}

	void CMultiEpoll::delFileEvent(INT fd, INT mask)
	{
		struct epoll_event ee = {0};

		ee.events = 0;
		mask = m_eventLoop->lstFileEvent[fd].mask & (~mask);
		if ( mask & NET_READABLE ) ee.events |= EPOLLIN;
		if ( mask & NET_WRITABLE ) ee.events |= EPOLLOUT;

		ee.data.fd = fd;
		if ( mask != NET_NONE ) {
			epoll_ctl(m_epfd, EPOLL_CTL_MOD, fd, &ee);
			LOG(INFO) << CLog::format("epoll mod file %d\n", fd);
		} else {
			epoll_ctl(m_epfd, EPOLL_CTL_DEL, fd, &ee);
			LOG(INFO) << CLog::format("epoll del file %d\n", fd);
		}
	}

	INT CMultiEpoll::eventLoop(void* timeout)
	{
		INT retval = 0;

		struct timeval* tvl = timeout ? (struct timeval*)timeout : NULL;

		retval = epoll_wait( m_epfd, m_events, m_eventLoop->size,  \
					tvl ? ( tvl->tv_sec * 1000 + tvl->tv_usec / 1000 ) : -1 );
		
		CHECK_R( retval != -1, -1 );
		LOG(INFO) << CLog::format("epoll: %d, size: %d! \n", retval, m_eventLoop->size);

		for ( INT index = 0; index < retval; ++index ) {
			INT mask = 0;
			struct epoll_event* e = m_events + index;

			if ( e->events & EPOLLIN )  mask |= NET_READABLE;
			if ( e->events & EPOLLOUT ) mask |= NET_WRITABLE;
			if ( e->events & EPOLLERR ) mask |= NET_WRITABLE;
			if ( e->events & EPOLLHUP ) mask |= NET_WRITABLE;

			m_eventLoop->lstFired[index].fd 	= e->data.fd;
			m_eventLoop->lstFired[index].type 	= ET_FILE; 
			m_eventLoop->lstFired[index].mask 	= mask;
		}
		return retval;
	}
}
