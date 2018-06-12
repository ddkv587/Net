#include "NET_Core.hpp"

namespace NET
{
	CSocket::CSocket()
		: m_fd(-1)
		, m_bIsOpenning(FALSE)
	{
        LOG_IF( ERROR, -1 == ( m_fd = socket(AF_INET, SOCK_STREAM, 0) ) )
            << CLog::format( "[%s, %d]  set socket to keep alive error: %s" ,__FILE__, __LINE__, strerror(errno) );
	}
	
	CSocket::~CSocket()
	{
		destroy();
	}

<<<<<<< Updated upstream
	void CSocket::init() noexcept
	{
		LOG_IF( ERROR, -1 == ( m_fd = socket(AF_INET, SOCK_STREAM, 0) ) )  
			<< CLog::format( "[%s, %d]  set socket to keep alive error: %s" ,__FILE__, __LINE__, strerror(errno) );
		m_bIsOpenning = TRUE;
	}

	void CSocket::destroy() noexcept
	{
		if ( m_bIsOpenning ) {
			close(m_fd);
			m_bIsOpenning = FALSE;
		}
	}

	void CSocket::setKeepAlive(BOOLEAN on, INT interval) noexcept
	{
		INT val = on ? 1 : 0;

		LOG_IF( ERROR, -1 == setsockopt(m_fd, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(val)) )
				<< CLog::format( "[%s, %d]  set socket to keep alive error: %s" ,__FILE__, __LINE__, strerror(errno) );	

		if ( !on ) return;

#ifndef OS_BSD
		val = interval;
		LOG_IF( ERROR, -1 == setsockopt(m_fd, IPPROTO_TCP, TCP_KEEPIDLE, &val, sizeof(val)) ) 
				<< CLog::format( "[%s, %d]  set socket to keep alive idle error: %s" ,__FILE__, __LINE__, strerror(errno) );	

		val = interval / 3;
		LOG_IF( ERROR, -1 == setsockopt(m_fd, IPPROTO_TCP, TCP_KEEPINTVL, &val, sizeof(val)) )
				<< CLog::format( "[%s, %d]  set socket to keep alive interval error: %s" ,__FILE__, __LINE__, strerror(errno) );	

		val = 3;
		LOG_IF( ERROR, -1 == setsockopt(m_fd, IPPROTO_TCP, TCP_KEEPCNT, &val, sizeof(val)) )
				<< CLog::format( "[%s, %d]  set socket to keep alive count error: %s" ,__FILE__, __LINE__, strerror(errno) );	
#else
        val = interval;
        LOG_IF( ERROR, -1 == setsockopt(m_fd, IPPROTO_TCP, TCP_KEEPALIVE, &val, sizeof(val)) ) 
				<< CLog::format( "[%s, %d]  set socket to keep alive error: %s" ,__FILE__, __LINE__, strerror(errno) );	

#endif
        }
	
	void CSocket::setTimeOut(INT timeout) noexcept
	{
#ifndef OS_BSD
		LOG_IF( ERROR, -1 == setsockopt(m_fd, IPPROTO_TCP, TCP_USER_TIMEOUT, &timeout, sizeof(timeout)) )
				<< CLog::format( "[%s, %d]  set socket timeout error: %s" ,__FILE__, __LINE__, strerror(errno) );	
#endif
	}
    
    void CSocket::setReusePort(BOOLEAN on) noexcept
    {
        INT val = on ? 1 : 0;

        LOG_IF( ERROR, -1 == setsockopt(m_fd, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val)) )
       			<< CLog::format( "[%s, %d]  set socket reuse port error: %s" ,__FILE__, __LINE__, strerror(errno) );
    }

	void CSocket::setReuseAddress(BOOLEAN on) noexcept
	{
		INT val = on ? 1 : 0;

		LOG_IF( ERROR, -1 == setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) )
				<< CLog::format( "[%s, %d]  set socket reuse addr error: %s" ,__FILE__, __LINE__, strerror(errno) );	
	}
	
	void CSocket::setNonBlock(BOOLEAN on) noexcept
	{
		INT flags = fcntl(m_fd, F_GETFL, 0);
		INT ret = on ? fcntl(m_fd, F_SETFL, flags | O_NONBLOCK) : fcntl(m_fd, F_SETFL, flags & ~O_NONBLOCK);

		LOG_IF( ERROR, -1 == ret )
				<< CLog::format( "[%s, %d]  set socket O_NONBLOCK error: %s" ,__FILE__, __LINE__, strerror(errno) );
	}

	void CSocket::setLinger(BOOLEAN on, INT val) noexcept
	{
	  	struct linger li;
  		li.l_onoff = on ? 1 : 0;
  		li.l_linger = val; 

		LOG_IF( ERROR, -1 == setsockopt(m_fd, SOL_SOCKET, SO_LINGER, &li, sizeof(li)) )
				<< CLog::format( "[%s, %d]  set socket SO_LINGER error: %s" ,__FILE__, __LINE__, strerror(errno) );
	}

	void CSocket::setNoDelay(BOOLEAN on) noexcept
	{
	  	INT val = on ? 1 : 0;

		LOG_IF( ERROR, -1 == setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val)) )
				<< CLog::format( "[%s, %d]  set socket TCP_NODELAY error: %s" ,__FILE__, __LINE__, strerror(errno) );
	}

	void CSocket::setSendBuffSize(INT64 size) noexcept
	{
		LOG_IF( ERROR, -1 == setsockopt(m_fd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) )
				<< CLog::format( "[%s, %d]  set socket SO_SNDBUF error: %s" ,__FILE__, __LINE__, strerror(errno) );
	}

	void CSocket::setRecvBuffSize(INT64 size) noexcept
	{
		LOG_IF( ERROR, -1 == setsockopt(m_fd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) )
				<< CLog::format( "[%s, %d]  set socket SO_RCVBUF error: %s" ,__FILE__, __LINE__, strerror(errno) );
	}
}
