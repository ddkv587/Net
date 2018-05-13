#include "NET_Listener.hpp"

namespace NET
{
	CListener::CListener()
        : m_pServer(NULL)
	{
        m_pServer = new CSocketServer();
        
		m_pServer.init();
	}

	CListener::~CListener()
	{
		;
	}

	void CListener::addFileListener(IFileListener* pListener)
	{
		assert(pListener != NULL);
		m_lstListener.push_back(pListener);
	}

	void CListener::delFileListener(IFileListener* pListener)
	{
		m_lstListener.remove(pListener);
	}
	
	void CListener::printListener()
	{
		::std::list<IFileListener*>::iterator itor = m_lstListener.begin();
		for(; itor != m_lstListener.end(); ++itor ) {
			LOG(INFO) << CLog::format( "processor %d size: %d\n", ::std::distance( m_lstListener.begin(), itor ), (*itor)->size() );
		}
	}
	
	IFileListener* CListener::balance(::std::list<IFileListener*>& lstListener)
	{
		CHECK_R(m_lstListener.size() != 0, NULL);	

		IFileListener* pListener = lstListener.front();
		lstListener.pop_front();
		lstListener.push_back(pListener);

		return pListener;
	}

	void CListener::mainLoop(void* arg)
	{
        if ( !m_pServer->bindAndListen() ) {
            LOG(ERROR) << CLog::format( "[%s, %d]  CListener bind and listen error, quit...", __FILE__, __LINE__ );
            return;
        }
        
        int client;
        struct sockaddr_in client_addr;
        int addLen = sizeof(struct sockaddr_in);

		while(!m_bStop)
		{
			if ( m_lstListener.empty() ) {
				LOG(WARNING) << CLog::format(" process thread not ready, sleep 10ms!! \n");
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}

			if ( -1 == ( client = accept(m_server.getSocketFD(), (struct sockaddr*)&client_addr, (socklen_t*)&addLen) ) ) {
				LOG(ERROR) << CLog::format( "[%s, %d]  accept error: %s" ,__FILE__, __LINE__, strerror(errno) );	
				continue;
			}

			LOG(INFO) << CLog::format("get socket from: %s, port: %d, clinet: %d\n", 
					inet_ntoa(client_addr.sin_addr),
					ntohs(client_addr.sin_port),
					client);

			setNonBlock(client);
			
			IFileListener* pListener = scheduling(m_lstListener);

			if ( NULL != pListener ) {
				pListener->addFileEvent(client, NET_READABLE);
			}
			printListener();
		}
	}

	void CListener::setNonBlock(int fd)
	{
		assert( fd > 0 );

		int flags = fcntl(fd, F_GETFL, 0);

		if ( flags == -1 ) 
			handle_error("fcntl get");

		flags |= O_NONBLOCK;

		if ( fcntl( fd, F_SETFL, flags ) == -1 ) 
			handle_error("fcntl set");
	}
}
