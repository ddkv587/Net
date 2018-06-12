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
		m_pMultiplex->enableEdgeTrigger();
#endif

		assert(NULL != m_pMultiplex);	
		LOG(INFO) << CLog::format( " %ld initialize, set size %d\n", getThreadID(), m_pMultiplex->setSize(1024) );
	}

	CProcessor::~CProcessor()
	{
		if ( NULL != m_pMultiplex ) {
			delete m_pMultiplex;
		}
	}

	INT CProcessor::addClient(int fd)
	{
        ::std::lock_guard(m_mutex);
		m_pMultiplex->addFileEvent(fd, NET_READABLE);
		++m_uiSize;
        
		return m_uiSize;
	}

	void CProcessor::delClient(int fd)
	{
        ::std::lock_guard(m_mutex);
        
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
				// reloop form multiplex
				EVENT_LOOP* eventLoop = m_pMultiplex->getEventLoop();

				for ( int index = 0; index < retval; ++index ) {
					FIRED_EVENT fired = eventLoop->fired[index];
					if( fired.mask == NET_READABLE ) {
                                            /*
						FILE_EVENT* file = &(eventLoop->event[fired.fd]);
                                              
						while (TRUE) {
							if ( NULL == file->data ) {
								file->data = new char(IProtocol::SIZE_HEADER_MANAGER);
								file->dataSize = 0;
							}

							if ( file->dataSize < IProtocol::SIZE_HEADER_MANAGER ) {
								int ret = recv(file->fd, file->data + file->dataSize, IProtocol::SIZE_HEADER_MANAGER - file->dataSize, 0);		

								if ( ret == -1 || ret == 0 ) goto err_recv;
								file->dataSize += ret;
							} 

							if ( file->dataSize >= IProtocol::SIZE_HEADER_MANAGER ) {
								IProtocol* protocol = new CProtocolBase();
								int ret = protocol->analyse(file->dataSize - IProtocol::SIZE_HEADER_MANAGER, (char*)file->data);

								if ( ret == -1 ) {
									//not support
									file->clean();
								} else {
									if ( ret != 0 ) {
										//not enough, need recv <ret> bytes
										if ( file->dataSize == IProtocol::SIZE_HEADER_MANAGER ) {
											IProtocol::HEADER_MANAGER* pHeader = (IProtocol::HEADER_MANAGER*)file->data;
											file->data = new char[pHeader->size + IProtocol::SIZE_HEADER_MANAGER];
											memcpy(file->data, pHeader, IProtocol::SIZE_HEADER_MANAGER);
											file->dataSize = IProtocol::SIZE_HEADER_MANAGER;		//next step is recv, increase or close
											delete []pHeader;
										}

										int size = recv(file->fd, file->data + file->dataSize, ret, 0);		
										if ( size == -1 || size == 0 ) goto err_recv;
										file->dataSize += size;

										if ( size != ret ) continue;
									}
								
									//complete and support, do protocol function
									IProtocol::HEADER_MANAGER* pHeader = (IProtocol::HEADER_MANAGER*)file->data;
									IProtocol::OBJECT* target = NULL;
									ret = protocol->callSpecialFunc(pHeader->protocol, \
																	file->dataSize - IProtocol::SIZE_HEADER_MANAGER, \
																	file->data + IProtocol::SIZE_HEADER_MANAGER, \
																	target);
									LOG(INFO) 
										<< CLog::format( "[%s, %d] callSpecialFunc end; %d, %p" ,__FILE__, __LINE__, ret, target);

									if ( ret == 0 && NULL != target ) {
										//need send 
										char* sendBuff = NULL;
										int sendSize = protocol->package(pHeader->protocol, target, sendBuff);
										
										if ( -1 == sendSize ) continue;
										LOG(INFO) 
												<< CLog::format( "[%s, %d] begin to send size; %d" ,__FILE__, __LINE__, sendSize );	
										while ( ( ret += send(file->fd, sendBuff + ret, sendSize - ret, 0) ) != sendSize ) {
											LOG(INFO) 
												<< CLog::format( "[%s, %d] send size; %d" ,__FILE__, __LINE__, ret );	
										}
										delete []sendBuff;
									}
									if ( NULL != target ) delete target;

									file->clean();
								} 
							}
							continue;
err_recv:
							if ( errno == EAGAIN ) break;

							close(file->fd);
							file->clean();
							delFileEvent(file->fd, NET_READABLE);
						}
                                              */
					}              
				} //for
			} //if retval > 0
		} //while
	}
}
