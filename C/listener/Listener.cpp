#include "NET_Listener.hpp"

namespace NET
{
	CListener::CListener()
		: m_socketFD(-1)
	{
		m_socketFD = socket(AF_INET, SOCK_STREAM, 0);
		assert(-1 != m_socketFD);

		int on = 1;
		setsockopt(m_socketFD, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

		struct sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(SYSTEM_SOCKET_PORT);

		assert( -1 != bind(m_socketFD, (struct sockaddr*)&address, sizeof(struct sockaddr_in) ));

		assert( -1 != listen(m_socketFD, 10) );
	}

	CListener::~CListener()
	{
		if ( m_socketFD > 0 ) {
			close(m_socketFD);
		}
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
			printf( "processor%d size: %d\n", ::std::distance( m_lstListener.begin(), itor ), (*itor)->size() );
		}
	}
	
	IFileListener* CListener::scheduling(::std::list<IFileListener*>& lstListener)
	{
		CHECK_R(m_lstListener.size() != 0, NULL);	

		IFileListener* pListener = lstListener.front();
		lstListener.pop_front();
		lstListener.push_back(pListener);

		return pListener;
	}

	void CListener::mainLoop(void* arg)
	{
		int client;
		struct sockaddr_in client_addr;
		int addLen = sizeof(struct sockaddr_in);

		while(!m_bStop)
		{
			if ( m_lstListener.empty() ) {
				printf(" process thread not ready!! \n");
				std::this_thread::sleep_for(std::chrono::milliseconds(10));
				continue;
			}

			client = accept(m_socketFD, (struct sockaddr*)&client_addr, (socklen_t*)&addLen);

			if ( client == -1 ) {
				perror("accept");
			}
			printf("get socket from :%s: %d, clinet: %d\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port, client);

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
		assert(fd > 0);

		int flags = fcntl(fd, F_GETFL, 0);

		if ( flags == -1 ) 
			handle_error("fcntl get");

		flags |= O_NONBLOCK;

		if ( fcntl(fd, F_SETFL, flags) == -1 ) 
			handle_error("fcntl set");
	}
}
