#include "NET_Socket.hpp"

namespace NET
{
	CSocket::CSocket()
		: fd(-1)
	{
		
	}
	
	CSocket::~CSocket()
	{
		if ( fd ) close(fd);
	}

	void CSocket::setKeepAlive(bool on, int idle, int interval, int count)
	{
		LOG_IF( CLog::LL_ERROR, -1 == setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &on, sizeof(on)) 
				<< CLog::format("[%s, %s], set socket to keep alive error: %s", __FILE__, __LINE__, perror(errno)) );	
		
		LOG_IF( CLog::LL_ERROR, -1 == setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &idle, sizeof(idle)) 
				<< CLog::format("[%s, %s], set socket to keep alive idle error: %s", __FILE__, __LINE__, perror(errno)) );	

		LOG_IF( CLog::LL_ERROR, -1 == setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &interval, sizeof(interval))
				<< CLog::format("[%s, %s], set socket to keep alive interval error: %s", __FILE__, __LINE__, perror(errno)) );	

		LOG_IF( CLog::LL_ERROR, -1 == setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &count, sizeof(count))
				<< CLog::format("[%s, %s], set socket to keep alive count error: %s", __FILE__, __LINE__, perror(errno)) );	
	}
	
	void CSocket::setTimeOut(int timeout)
	{
		LOG_IF( CLog::LL_ERROR, -1 == setsockopt(fd, IPPROTO_TCP, TCP_USER_TIMEOUT, &timeout, sizeof(timeout))
				<< CLog::format("[%s, %s], set socket timeout error: %s", __FILE__, __LINE__, perror(errno)) );	
	}
}
