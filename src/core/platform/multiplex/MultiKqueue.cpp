#ifdef OS_BSD

#include "NET_Core.hpp"

namespace  NET
{
    CMultiKqueue::CMultiKqueue()
    : m_kqfd(-1)
    , m_events(nullptr)
    {
        m_kqfd = kqueue();
        CHECK( -1 != m_kqfd);
        
        m_eType = EMT_KQUEUE;
    }

    CMultiKqueue::~CMultiKqueue()
    {
        close(m_kqfd);

        if ( NULL != m_events ) {
            delete m_events;
        }
    }

    int CMultiKqueue::setSize(int size)
    {
        size = CMultiBase::setSize(size);
        
        if ( NULL != m_events ) {
            delete m_events;
        }
        m_events = new struct kevent[size];
        
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

    int CMultiKqueue::eventLoop(void* timeout)
    {
        int retval = 0;
        
        if ( NULL != timeout ) {
            struct timespec ts;
            ts.tv_sec = static_cast<struct timeval*>(timeout)->tv_sec;
            ts.tv_nsec = static_cast<struct timeval*>(timeout)->tv_usec * 1000;
            retval = kevent(m_kqfd, NULL, 0, m_events, m_eventLoop->size, &ts);
        } else {
            retval = kevent(m_kqfd, NULL, 0, m_events, m_eventLoop->size, NULL);
        }
        
        for(int i = 0; i < retval; ++i) {
            int mask = 0;
            struct kevent *e = m_events + i;

            if (e->filter == EVFILT_READ) mask |= NET_READABLE;
            if (e->filter == EVFILT_WRITE) mask |= NET_WRITABLE;
            
            m_eventLoop->fired[i].fd = e->ident;
            m_eventLoop->fired[i].fd = mask;
        }
        return retval;
    }
}
#endif
