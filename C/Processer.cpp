#include <unistd.h>
#include <sys/socket.h>
#include <iostream>
#include "Processer.hpp"
#include "Protocol.hpp"
#include "common.h"

namespace NET
{
	CProcessor::CProcessor()
		: m_pMultiplex(NULL)
	{
		m_pMultiplex = new CMultiplexManager(EMT_EPOLL);

		assert(NULL != m_pMultiplex);
	}

	CProcesser::~CProcesser()
	{
		if ( NULL != m_pMultiplex ) {
			delete m_pMultiplex;
		}
	}

	int CProcesser::addFileEvent(int fd, int mask) 
	{
		m_pMultiplex->addFileEvent(fd, mask);
		return 0;
	}

	void CProcesser::delFileEvent(int fd, int mask)
	{
		m_pMultiplex->delFileEvent(fd, mask);
	}

	void CProcesser::run()
	{
		m_thread = std::thread(&CProcesser::mainLoop, this);
		m_thread.detach();

		std::cout << "start thread: %d" << getThreadID();
	}

	void CProcesser::mainLoop() 
	{
		struct timespec timeout;

		while(!s_iStop)
		{
			timeout.tv_sec = POLL_TIMEOUT_SEC;
			timeout.tv_nsec = POLL_TIMEOUT_USEC * 1000;

			int retval = m_pMultiplex->eventLoop(&timeout);
			if ( retval > 0 ) {
				EVENT_LOOP* eventLoop = m_pMultiplex->m_pEventLoop;
				for ( int index = 0; index < retval; ++index ) {
					FIRED_EVENT* event = eventLoop->fired[index];
					if( event->mask == NET_READABLE ) {
						FILE_EVENT* fileEvent = eventLoop->event[index];

						if ( dataSzie > 0 ) {
							recv(event)
						}	


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

								printf("recv data size: %d\n", header->length);
								char* buff = new char(header->length);
								size = recv(e->ident, buff, header->length, 0);

								if ( size != header->length ) {
									printf("read error");	
								} else {
									switch( header->protocol ) {
									case EP_ECHO:
										printf("recv echo msg: %s\n", buff);
										size = send(e->ident, buff, size, 0);
										printf("send size: %d\n", size);
										break;
									case EP_PING:
										{
											printf("recv ping msg: %s\n", buff + sizeof(PING_MANAGER));
											PING_MANAGER* ping_header = (PING_MANAGER*)buff;
											if ( ping_header->current < ping_header->deadLimit ) {
												printf("recv ping current: %d\n", ping_header->current);
												ping_header->current += 1;

												char* sendBuff = (char *)malloc(size + sizeof(HEADER_MANAGER));

												memcpy(sendBuff, header, sizeof(HEADER_MANAGER));
												memcpy(sendBuff+sizeof(HEADER_MANAGER), buff, size);

												size = send(e->ident, sendBuff, size + sizeof(HEADER_MANAGER), 0);
												printf("send size: %d\n", size);

												free(sendBuff);
											}
										}
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
	}
}
