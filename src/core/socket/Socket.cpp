#include "NET_Core.hpp"

namespace NET
{
	CSocket::CSocket()
		: m_fd(-1)
	{
		;	
	}
	
	CSocket::~CSocket()
	{
		if ( m_fd ) close(m_fd);
	}

	void CSocket::init()
	{
		LOG_IF( ERROR, -1 == ( m_fd = socket(AF_INET, SOCK_STREAM, 0) ) )  
			<< CLog::format( "[%s, %d]  set socket to keep alive error: %s" ,__FILE__, __LINE__, strerror(errno) );
	}

	void CSocket::destroy()
	{
		;
	}

	void CSocket::setKeepAlive(bool on, int interval)
	{
		int val = 0;

		on ? val = 1 : val = 0;
		LOG_IF( ERROR, -1 == setsockopt(m_fd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val)) )
				<< CLog::format( "[%s, %d]  set socket to keep alive error: %s" ,__FILE__, __LINE__, strerror(errno) );	

		if ( !on ) return;

		val = interval;
		LOG_IF( ERROR, -1 == setsockopt(m_fd, IPPROTO_TCP, TCP_KEEPIDLE, &val, sizeof(val)) ) 
				<< CLog::format( "[%s, %d]  set socket to keep alive idle error: %s" ,__FILE__, __LINE__, strerror(errno) );	

		val = interval / 3;
		LOG_IF( ERROR, -1 == setsockopt(m_fd, IPPROTO_TCP, TCP_KEEPINTVL, &val, sizeof(val)) )
				<< CLog::format( "[%s, %d]  set socket to keep alive interval error: %s" ,__FILE__, __LINE__, strerror(errno) );	

		val = 3;
		LOG_IF( ERROR, -1 == setsockopt(m_fd, IPPROTO_TCP, TCP_KEEPCNT, &val, sizeof(val)) )
				<< CLog::format( "[%s, %d]  set socket to keep alive count error: %s" ,__FILE__, __LINE__, strerror(errno) );	
	}
	
	void CSocket::setTimeOut(int timeout)
	{
		LOG_IF( ERROR, -1 == setsockopt(m_fd, IPPROTO_TCP, TCP_USER_TIMEOUT, &timeout, sizeof(timeout)) )
				<< CLog::format( "[%s, %d]  set socket timeout error: %s" ,__FILE__, __LINE__, strerror(errno) );	
	}
    
    void CSocket::setReusePort(bool on)
    {
        int val = 0;
        
        on ? val = 1 : val = 0;
        LOG_IF( ERROR, -1 == setsockopt(m_fd, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val)) )
        << CLog::format( "[%s, %d]  set socket reu e port error: %s" ,__FILE__, __LINE__, strerror(errno) );
    }

	void CSocket::setReuseAddress(bool on)
	{
		int val = 0;

		on ? val = 1 : val = 0;
		LOG_IF( ERROR, -1 == setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) )
				<< CLog::format( "[%s, %d]  set socket reuue addr error: %s" ,__FILE__, __LINE__, strerror(errno) );	
	}
}
