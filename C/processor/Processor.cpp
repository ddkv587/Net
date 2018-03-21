#include "NET_Processor.hpp"

namespace NET
{
	CProcessor::CProcessor()
		: m_uiSize(0)
		, m_pMultiplex(NULL)
	{
#ifdef OS_BSD
		m_pMultiplex = new CMultiKqueue();
#else
		m_pMultiplex = new CMultiEpoll();
#endif
		
		assert(NULL != m_pMultiplex);

		printf( " set size %d\n", m_pMultiplex->setSize(1024) );
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
		
		++m_uiSize;
		return 0;
	}

	void CProcessor::delFileEvent(int fd, int mask)
	{
		m_pMultiplex->delFileEvent(fd, mask);
		
		m_uiSize > 0 ? --m_uiSize : 0;
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
					FIRED_EVENT fired = eventLoop->fired[index];
					if( fired.mask == NET_READABLE ) {
						FILE_EVENT file = eventLoop->event[fired.fd];
						// int ret = file->readProc(fired->fd, file->data, file->dataSzie);
						char buff[1024];
						int ret = recv(fired.fd, buff, 1024, 0);
						if ( ret == -1 || ret == 0) {
							close(fired.fd);
							delFileEvent(fired.fd, NET_READABLE);
							printf("close socket: %d\n", fired.fd);
							continue;
						} else {
							printf("recv msg: %s, size: %d\n", buff, ret);
						}
					}
				} //for
			} //if retval > 0
		} //while
	}
}
