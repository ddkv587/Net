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
						int ret = file->readProc(fired->fd, file->data, file->dataSzie);
						
						char buff[1024];
						int ret = recv(fired.fd, buff, 1024, 0);
						if ( ret == -1 || ret == 0) {
							close(fired.fd);
							delFileEvent(fired.fd, NET_READABLE);
							continue;
						} else {
							if ( file->dataSzie > SIZE_HEADER_MANAGER ) {
								IProtocol protocol;
								int type = protocol.analyse(file->data);
								
								if ( type == -1 ) {
									//support but not enough, wait for next loop
									;
								} else if ( type == 0 ) {
									//not support
									file.clean();
								} else {
									OBJECT* target = NULL;
									ret = protocol.callSpecialFunc(type, target);
									if ( ret == 0 && NULL != target ) {
										//need send 
										int sendSize = target.size + SIZE_HEADER_MANAGER;
										char* sendBuff = new char(sendSize);
										protocol.package(type, target, sendBuff);
										ret = 0;
										while ( ( ret += send(fired.fd, sendBuff + ret, sendSize - ret, 0) ) == sendSize );
										delete sendBuff;
									}
									if ( NULL != target ) delete target;
								}
							}
						}
					}
				} //for
			} //if retval > 0
		} //while
	}
}
