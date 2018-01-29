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
					FIRED_EVENT* fired = eventLoop->fired[index];
					if( fired->mask == NET_READABLE ) {
						FILE_EVENT* file = eventLoop->event[fired->fd];

						if ( file->dataSzie < SIZE_HEADER_MANAGER ) {
							if ( file->data == NULL ) file->data = new char(SIZE_HEADER_MANAGER);
							int size = recv(fired->fd, file->data, SIZE_HEADER_MANAGER - file->dataSzie, 0);
							if ( size == -1 || size == 0 ) {
								close(fired->fd);
								delEvent(fired->fd, NET_READABLE);
								continue;
							}
							file->dataSzie += size;
							if ( file->dataSzie != SIZE_HEADER_MANAGER ) continue;
						}
						HEADER_MANAGER* header = (HEADER_MANAGER*)file->data;

						if ( header->sync & 0x03302112 ) {
							if ( header->protocol == EP_DISMISS ) break;
							if ( file->dataSzie == SIZE_HEADER_MANAGER ) {
								char *old = file->data;
								file->data = new char(SIZE_HEADER_MANAGER + header->length);
								memcpy(file->data, old, SIZE_HEADER_MANAGER);
								header = file->data;
								delete old;
							}
							char* buff = file->data + SIZE_HEADER_MANAGER;
							int size = recv(fired->fd, buff, header->length - (file->dataSzie - SIZE_HEADER_MANAGER), 0);
							if ( size == -1 || size == 0 ) {
								close(fired->fd);
								delEvent(fired->fd, NET_READABLE);
								continue;
							}
							file->dataSzie += size;

							if ( size != header->length ) continue; 

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
						file->dataSzie = 0;
						delete file->data;
					}
				}
			}
		}
	}
}
