#include "NET_Socket.hpp"

namespace NET
{
	CSocketServer::CSocketServer()
		: m_serverIp("0.0.0.0")
		, m_serverPort(8000)
	{
		;
	}

	CSocketServer::~CSocketServer()
	{
		;
	}

	void CSocketServer::init()
	{
		LOG_IF( ERROR, -1 == ( m_fd = socket(AF_INET, SOCK_STREAM, 0) ) )  
			<< CLog::format( "[%s, %d]  set socket to keep alive error: %s" ,__FILE__, __LINE__, strerror(errno) );
	}

	void CSocketServer::destroy()
	{
		;
	}

	void CSocketServer::bindAndListen()
	{
		CHECK( -1 != m_fd );

		struct sockaddr_in address;

		address.sin_family = AF_INET;
		address.sin_port = htons(m_serverPort);
		LOG_IF( ERROR, INADDR_NONE == ( address.sin_addr.s_addr = inet_addr(m_serverIp) ) )  
			<< CLog::format( "[%s, %d]  server ip is error: %s" ,__FILE__, __LINE__, strerror(errno) );
	
		CHECK( -1 != bind( m_fd, (struct sockaddr*)&address, sizeof(struct sockaddr_in) ) );
		
		CHECK( -1 != listen( m_fd, SOMAXCONN) );
	}		
}
