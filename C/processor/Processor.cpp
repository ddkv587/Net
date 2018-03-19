#include "NET_Processor.hpp"

namespace NET
{
	CProcessor::CProcessor()
		: m_pMultiplex(NULL)
	{
#ifdef OS_BSD
		m_pMultiplex = new CMultiKqueue();
#else
		m_pMultiplex = new CMultiEpoll();
#endif
		
		assert(NULL != m_pMultiplex);

		m_pMultiplex->setSize(1024);
	}

	CProcessor::~CProcessor()
	{
		if ( NULL != m_pMultiplex ) {
			delete m_pMultiplex;
		}
	}

	int CProcessor::addFileEvent(int fd, int mask) 
	{
		m_pMultiplex->addFileEvent(fd, mask);
		return 0;
	}

	void CProcessor::delFileEvent(int fd, int mask)
	{
		m_pMultiplex->delFileEvent(fd, mask);
	}

	void CProcessor::mainLoop(void* arg) 
	{
		struct timeval timeout;

		while(!m_bStop)
		{
			timeout.tv_sec = POLL_TIMEOUT_SEC;
			timeout.tv_usec = POLL_TIMEOUT_USEC;

			int retval = m_pMultiplex->eventLoop(&timeout);
			if ( retval > 0 ) {
				EVENT_LOOP* eventLoop = m_pMultiplex->getEventLoop();
			
				for ( int index = 0; index < retval; ++index ) {
					FIRED_EVENT* fired = eventLoop->fired + index;
					if( fired->mask == NET_READABLE ) {
						FILE_EVENT* file = eventLoop->event + fired->fd;
						int ret = 0; //file->readProc(fired->fd, file->data, file->dataSzie);

						if ( ret == -1 ) {
							close(fired->fd);
							delEvent(fired->fd, NET_READABLE);
							continue;
						} else

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
