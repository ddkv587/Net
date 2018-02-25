#include <iostream>
#include <thread>
#include <chrono>
#include <functional>
#include <sys/types.h>
#include <sys/times.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "common.h"
#include "Listener.hpp"

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

void CListener::addFileListener(const IFileListener* pListener)
{
	assert(pListener != NULL)
	m_lstListener.push_back(pListener);
}

void CListener::delFileListener(const IFileListener* pListener)
{
	m_lstListener.remove(pListener);
}

void CListener::mainLoop()
{
	int client;
	struct sockaddr_in client_addr;
	int addLen = sizeof(struct sockaddr_in);
	char readBuff[1024];
	fd_set rfds;
	int ret;

	while(!s_iStop)
	{
		if ( m_lstListener.empty() ) {
			std::cout << " process thread not ready!! " << std::endl;
			std::this_thread::sleep_for(std::chrono::millsecond(10));
			continue;
		}

		client = accept(m_socketFD, (struct sockaddr*)&client_addr, (socklen_t*)&addLen);

		if ( client == -1 ) {
			perror("accept");
		}
		printf("get socket from :%s: %d\n", inet_ntoa(client_addr.sin_addr), client_addr.sin_port);
	
		setNonBlock(client);

		IFileListener* pListener = m_lstListener.front();
		pListener.addFileEvent(client, NET_READABLE);
		m_lstListener.pop_front();
		m_lstListener.push_back(pListener);
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

