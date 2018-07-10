#include "NET_Worker.hpp"

namespace NET
{
	INT CWorker::CProcessor::read(const EVENT_LOOP* loop, INT fd, DATA*& dataBuff, INT mask)
	{
		INT rcvSize = 0;
		//while ( ET )
		{
			if ( dataBuff == nullptr ) {
				dataBuff 			= new DATA;
				dataBuff->buffer 	= new char[IProtocol::SIZE_HEADER_MANAGER];
				dataBuff->uiSize 	= 0;		
			}

			if ( dataBuff->uiSize < IProtocol::SIZE_HEADER_MANAGER ) {
				rcvSize = recv(fd, (CHAR*)dataBuff->buffer + dataBuff->uiSize, IProtocol::SIZE_HEADER_MANAGER - dataBuff->uiSize, 0);		

				if ( rcvSize == -1 || rcvSize == 0 ) goto err_recv;
				dataBuff->uiSize += rcvSize;
			}

			if ( dataBuff->uiSize >= IProtocol::SIZE_HEADER_MANAGER ) {
				IProtocol* protocol = new CProtocolBase();
				INT ret = protocol->analyse((CHAR*)dataBuff->buffer, dataBuff->uiSize );

				if ( ret == -1 ) {
					//not support, dismiss 
					delete (CHAR*)dataBuff->buffer;				
					delete dataBuff;
					dataBuff = nullptr;
				} else {
					if ( ret != 0 ) {
						//not enough, need recv <ret> bytes
						if ( dataBuff->uiSize == IProtocol::SIZE_HEADER_MANAGER ) {
							IProtocol::HEADER_MANAGER* pHeader = (IProtocol::HEADER_MANAGER*)dataBuff->buffer;
							dataBuff->buffer = new CHAR[pHeader->uiSize + IProtocol::SIZE_HEADER_MANAGER];		//remalloc new size: header + data
							memcpy(dataBuff->buffer, pHeader, IProtocol::SIZE_HEADER_MANAGER);			
							delete []pHeader;	//delete header new before
						}
						//next step is recv, dataBuff->uiSize must be changed
						rcvSize = recv(fd, (CHAR*)dataBuff->buffer + dataBuff->uiSize, ret, 0);		
						if ( rcvSize == -1 || rcvSize == 0 ) goto err_recv;
						dataBuff->uiSize += rcvSize;

						//if ( rcvSize != ret ) continue;
					}
				}
			}
		} // ET 
		return 0;
err_recv:
		if ( nullptr != dataBuff ) {
			if ( nullptr != dataBuff->buffer ) 
				delete (CHAR*)dataBuff->buffer;

			delete dataBuff;
			dataBuff = nullptr;
		}
		if ( 0 == rcvSize ) {
			// socket close other side
			return -1;
		}
		return errno;
	}

	INT CWorker::CProcessor::write(const EVENT_LOOP* loop, INT fd, DATA*& dataBuff, INT mask)
	{
		LOG(INFO) << "call write proc \n";

		return 0;
	}

	CWorker::CWorker()
		: m_uiSize(0)
		, m_pMultiManager(nullptr)
		, m_mutex()
	{
		m_pMultiManager = new CMultiManager();
		assert(NULL != m_pMultiManager);

		m_pMultiManager->init();
		LOG(INFO) << CLog::format( " %ld initialized  \n" );
		//, set size %d\n", getThreadID(), m_pMultiManager->setSize(1024) );
	}

	CWorker::~CWorker()
	{
		if ( NULL != m_pMultiManager ) {
			m_pMultiManager->destroy();

			delete m_pMultiManager;
		}
	}

	INT CWorker::addClient(INT fd)
	{  
		const EVENT_LOOP* eventLoop = CMultiManager::getEventLoop();

		m_pMultiManager->addFileEvent(fd, NET_READABLE, CWorker::CProcessor::read, eventLoop->lstFileEvent[fd].clientData);
		m_pMultiManager->addFileEvent(fd, NET_WRITEABLE, CWorker::CProcessor::write, eventLoop->lstFileEvent[fd].clientData);

		::std::lock_guard<::std::mutex> lock(m_mutex);
		++m_uiSize;
        
		return m_uiSize;
	}

	void CWorker::delClient(INT fd)
	{      
		m_pMultiManager->delFileEvent(fd, NET_READABLE);

		::std::lock_guard<::std::mutex> lock(m_mutex);
		m_uiSize > 0 ? --m_uiSize : 0;
	}

	void CWorker::setMaxSize(UINT uiSize)
	{
		if ( nullptr != m_pMultiManager ) m_pMultiManager->setSize(uiSize);
	}

	void CWorker::mainLoop(void* arg) 
	{
		struct timeval timeout;

		while(!m_bStop)
		{
			timeout.tv_sec = POLL_TIMEOUT_SEC;
			timeout.tv_usec = POLL_TIMEOUT_USEC;

			int retval = m_pMultiManager->eventLoop(&timeout);
			if ( retval > 0 ) {
				// reloop form multiplex
				const EVENT_LOOP* eventLoop = CMultiManager::getEventLoop();

				for ( int index = 0; index < retval; ++index ) {
					FIRED_EVENT fired = m_pMultiManager->getFiredList()[index];

					if ( fired.type == ET_FILE ) {
						FILE_EVENT* file = &(eventLoop->lstFileEvent[fired.index]);
						if( fired.mask & NET_READABLE ) {
							INT ret = file->readProc( eventLoop, fired.index, file->clientData, INT(NET_READABLE) );
							if ( 0 == ret ) {
								// read complete
								
							} else if ( ret > 0 ) {
								// read but not enough
								
							} else {
								// error, clean file and close
								goto err_read_proc;
							}
						}
						if( fired.mask & NET_WRITEABLE ) {
							file->writeProc(eventLoop, fired.index, file->clientData, NET_READABLE);


						}
						continue;
err_read_proc:
						if ( nullptr != file->clientData ) {
							delete file->clientData;
							file->clientData = nullptr;
						}
						file->readProc 	= nullptr;
						file->writeProc = nullptr;
						file->mask		= NET_NONE;

						delClient(fired.index);
					} // ET_FILE
				} // for
			} // retval > 0
		} // while
	}

	void CWorker::process( void*& dataBuffer )
	{
		
	}
}
