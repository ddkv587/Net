#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include <thread>
#include "Processer.hpp"
#include "Protocol.hpp"

//namespace NET
//{
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
	
	void CProcesser::delEvent(int fd, int mask)
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
		std::thread(&CProcesser::mainLoop, this).join();
	}

	void* CProcesser::mainLoop(void *argv) 
	{
		struct timespec timeout;

		while(!s_iStop)
		{
		timeout.tv_sec = POLL_TIMEOUT_SEC;
		timeout.tv_nsec = POLL_TIMEOUT_USEC * 1000;

		int retval = kevent(m_kqfd, NULL, 0, m_events, SYSTEM_LIMIT_MAX_EPOLL_EVENTS, &timeout);
	
		if ( retval > 0 ) {
			for ( int index = 0; index < retval; ++index ) {
				struct kevent* e = m_events + index;
				if( e->filter == EVFILT_READ ) {
					HEADER_MANAGER* header = new HEADER_MANAGER();
					int size = recv(e->ident, header, sizeof(HEADER_MANAGER), 0);
					switch ( size ) {
					case -1:
						perror("recv");
					case 0:
						close(e->ident);
						delEvent(e->ident, NET_READABLE);
						break;
					default:
						if ( header->sync & 0x03302112 ) {
							if ( header->protocol == EP_DISMISS ) break;
							
							char* buff = new char(header->length);
							int size = recv(e->ident, buff, header->length, 0);
							if ( size != header->length ) {
								printf("read error");	
							} else {
								switch( header->protocol ) {
								case EP_ECHO:
									send(e->ident, buff, size, 0);
									break;
								case EP_PING:
									break;
								case EP_TIME:
									break;
								case EP_HEART:
									break;
								default:break;
								}
							}
							delete buff;
						} else {
							printf("sync error");
						}
						break;
					}
					delete header;
				} else {
				
				}
			}
		}
		}
		return NULL;
	}
//}
