#include <sys/time.h>
#include "MultiKqueue.hpp"

CMultiKqueue::CMultiKqueue(int size)
{
	m_events = new struct kevent[size];

	m_kqfd = kqueue();
	assert( NULL != m_events && -1 != m_kqfd);
}

CMultiKqueue::~CMultiKqueue()
{
	close(m_kqfd);

	delete m_events;
}

int CMultiKqueue::resize(int size)
{
	delete m_events;
	m_events = NULL;

	m_events = new struct kevent[size];
	assert( NULL != m_events );

	return size;
}

int CMultiKqueue::addFileEvent(int fd, int mask)
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

void CMultiKqueue::delFileEvent(int fd, int mask)
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

int CMultiKqueue::eventLoop(EVENT_LOOP *loop, void* timeout)
{
	int retval = 0;
	if ( NULL != timeout ) {
		struct timespec ts;
		ts.tv_sec = (struct timeval*)timeout->tv_sec;
		ts.tv_nsec = (struct timeval*)timeout->tv_usec * 1000;
		retval = kevent(m_kqfd, NULL, 0, m_events, loop->size, &ts);
	} else {
		retval = kevent(m_kqfd, NULL, 0, m_events, loop->size, NULL);
	}
	
	if (retval > 0) {
        for(int i = 0; i < retval; ++i) {
            int mask = 0;
            struct kevent *e = m_events + i;

            if (e->filter == EVFILT_READ) mask |= NET_READABLE;
            if (e->filter == EVFILT_WRITE) mask |= NET_WRITABLE;
            loop->fired[j].fd = e->ident;
            loop->fired[j].mask = mask;
        }
    }
    return retval;
}
