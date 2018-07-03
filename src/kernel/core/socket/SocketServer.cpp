#include "NET_Core.hpp"

namespace NET
{
	CSocketServer::CSocketServer(STRING ip, UINT port)
		: m_strIP(ip)
		, m_uiPort(port)
	{
        if ( ip == STRING_NULL ) m_strIP = "0.0.0.0";
	}

	CSocketServer::~CSocketServer()
	{
		;
	}

	void CSocketServer::init() noexcept
	{
		CSocket::init();		

		setReuseAddress(TRUE);	
		setNonBlock(TRUE);
		setLinger(FALSE, 0);
		setNoDelay(FALSE);

		setSendBuffSize(1 * 1024 * 1024);		//1M
		setRecvBuffSize(1 * 1024 * 1024);		//1M
	}

	void CSocketServer::destroy() noexcept
	{
		CSocket::destroy();
	}

	BOOLEAN CSocketServer::bindAndListen()
	{
		CHECK( -1 != m_fd );

		struct sockaddr_in address;

		address.sin_family = AF_INET;
		address.sin_port = htons(m_uiPort);
		LOG_IF( ERROR, INADDR_NONE == ( address.sin_addr.s_addr = inet_addr(m_strIP.data()) ) )
			<< CLog::format( "[%s, %d]  server ip is error: %s" ,__FILE__, __LINE__, strerror(errno) );
	
		CHECK_R( -1 != bind( m_fd, (struct sockaddr*)&address, sizeof(struct sockaddr_in) ), FALSE );
		
		CHECK_R( -1 != listen( m_fd, SOMAXCONN), FALSE );
        
        return TRUE;
	}		
}
