#include "Processer.hpp"

//namespace NET
{
	CProcesser::CProcesser()
	{
		m_kqfd = kqueue();	
		if ( m_kqfd == -1 )	
			handle_error("kqueue");


	}

	CProcesser::~CProcesser()
	{
	
	}

	int CProcesser::addEvent(int fd, int mask) 
	{
		struct kevent ke;

		if ( mask & NET_READABLE ) {
			EV_SET(&ke, fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
			if ( kevent(m_kqfd, &ke, 1, NULL, 0, NULL) == -1 ) return -1;
		}
		if ( mask & NET_WRITABLE ) {
			EV_SET(&ke, fd, EVFILT_WRITE, EV_ADD, 0, 0, NULL);
			if ( kevent(m_kqfd, &ke, 1, NULL, 0, NULL) == -1 ) return -1;
		}
		return 0;
	}
	
	void CProcesser::remveEvent(int fd)
	{
		struct kevent ke;

		if ( mask & NET_READABLE ) {
			EV_SET(&ke, fd, EVFILT_READ, EV_DELETE, 0, 0, NULL);
			kevent(m_kqfd, &ke, 1, NULL, 0, NULL);
		}
		if ( mask & NET_WRITABLE ) {
			EV_SET(&ke, fd, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
			kevent(m_kqfd, &ke, 1, NULL, 0, NULL);
		}
	}

	void CProcesser::run()
	{
		pthread_create(&m_thread.thread_id, );	
	}

	void* CProcesser::mainLoop(void *argv) 
	{
		struct timespec timeout;
		timeout.tv_sec = POLL_TIMEOUT_SEC;
		timeout.tv_nsec = POLL_TIMEOUT_USEC * 1000;

		int retval = kevent(m_kqfd, NULL, 0, m_events, SYSTEM_LIMIT_MAX_EPOLL_EVENTS, &timeout);
	
		if ( retval > 0 ) {
			for ( int index = 0; index < retval; ++index ) {
				//read();
				//parse();
				//
			}
		}

		return NULL;
	}
}
