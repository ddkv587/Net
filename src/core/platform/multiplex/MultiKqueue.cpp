#ifdef OS_BSD

#include "NET_Core.hpp"

namespace  NET
{
    CMultiKqueue::CMultiKqueue()
    : m_kqfd(-1)
    , m_events(nullptr)
    {
        m_eType = EMT_KQUEUE;
    }

    CMultiKqueue::~CMultiKqueue()
    {
		;
    }
	
	void CMultiKqueue::init()
	{
        m_kqfd = kqueue();
        CHECK( -1 != m_kqfd);
	}

	void CMultiKqueue::destroy()
	{
        if ( -1 != m_kqfd ) {
			close(m_kqfd);
			m_kqfd = -1;
		}

        if ( nullptr != m_events ) {
            delete m_events;
  			m_events = nullptr;
		}
	}

    UINT CMultiKqueue::setSize(UINT size)
    {
        if ( NULL != m_events ) {
            delete m_events;
        }
        m_events = new struct kevent[size];
        
        return size;
    }

    INT CMultiKqueue::addFileEvent(INT fd, INT mask, EVENT_LOOP* eventLoop)
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

    void CMultiKqueue::delFileEvent(INT fd, INT mask, EVENT_LOOP* eventLoop)
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

    INT CMultiKqueue::eventLoop(void* timeout, EVENT_LOOP* eventLoop)
    {
        INT retval = 0;
        
        if ( NULL != timeout ) {
            struct timespec ts;
            ts.tv_sec = static_cast<struct timeval*>(timeout)->tv_sec;
            ts.tv_nsec = static_cast<struct timeval*>(timeout)->tv_usec * 1000;
            retval = kevent(m_kqfd, NULL, 0, m_events, eventLoop->size, &ts);
        } else {
            retval = kevent(m_kqfd, NULL, 0, m_events, eventLoop->size, NULL);
        }
        
        for(INT i = 0; i < retval; ++i) {
            INT mask = 0;
            struct kevent *e = m_events + i;

            if (e->filter == EVFILT_READ) mask |= NET_READABLE;
            if (e->filter == EVFILT_WRITE) mask |= NET_WRITABLE;
            
            eventLoop->lstFired[i].index 	= e->ident;
            eventLoop->lstFired[i].type 	= ET_FILE;
            eventLoop->lstFired[i].mask     = mask;
        }
        return retval;
    }
}
#endif
