#include "NET_Processor.hpp"

namespace NET
{
	INT CWorker::CProcessor::read(EVENT_LOOP* loop, INT fd, DATA*& dataBuff, INT mask)
	{
		if ( dataBuff == NULL ) {
			dataBuff = (DATA*)new CHAR[IProtocol::SIZE_HEADER_MANAGER + 4];
			dataBuff->size = 0;		
		}

		if ( dataBuff->size < IProtocol::SIZE_HEADER_MANAGER ) {
			int ret = recv(fd, dataBuff->buffer + dataBuff->size, IProtocol::SIZE_HEADER_MANAGER - dataBuff->size, 0);		

			if ( ret == -1 || ret == 0 ) return -1;
			dataBuff->size += ret;
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
			}
		}
	}

	INT CWorker::CProcessor::write(EVENT_LOOP* loop, INT fd, DATA*& dataBuff, INT mask)
	{

	}

	CWorker::CWorker()
		: m_uiSize(0)
		, m_pMultiManager(nullptr)
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
		m_pMultiManager->addFileEvent(fd, NET_READABLE, CProcessor::read, s_pEventLoop[fd].clientData);
		m_pMultiManager->addFileEvent(fd, NET_WRITEABLE, CProcessor::write, s_pEventLoop[fd].clientData);

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
					FIRED_EVENT fired = eventLoop->lstFired[index];

					if ( fired.type == ET_FILE ) {
						FILE_EVENT* file = &(eventLoop->lstFileEvent[fired.index]);
						if( fired.mask & NET_READABLE ) {
							INT ret = file->readProc(eventLoop, fired.index, file->clientData, NET_READABLE) );
							if ( 0 == ret ) {
								// read complete
								
							} else if ( ret > 0 ) {
								// read but not enough
								
							} else {
								// error, clean file and close
								goto err_clear_file;
							}
						}
						if( fired.mask & NET_WRITEABLE ) {
							file->writeProc(eventLoop, fired.index, file->clientData, NET_READABLE);
						}
						continue;
err_clear_file:
						if ( nullptr != file->clientData ) {
							delete file->clientData;
							file->clientData = nullptr;
						}
						file->readProc 	= nullptr;
						file->writeProc = nullptr;
						file->mask		= NET_NONE;
					} // ET_FILE
				} // for
			} // retval > 0
		} // while
	}

	void CWorker::process( void*& dataBuffer )
	{
		
	}
}
