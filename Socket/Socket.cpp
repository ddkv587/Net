#include <assert.h>
#include "Socket.hpp"

namespace Net 
{
	CSocket::CSocket() : m_socketfd(-1) {}

	CSocket::CSocket(CSocket&& sock) : m_socketfd(sock.m_socketfd) {}

	int CSocket::~CSocket()
	{
		return destroy();
	}

	int CSocket::destroy()
	{
		if ( m_socketfd > 0 ) {
			return close(m_socketfd);	
		}
		m_socketfd = -1;
		return 0;
	}

	int CSocket::setSocketOpt(int level, int optname, const char* optval, socketlen_t optlen)
	{
		assert ( m_socketfd > 0 );
		return setsockopt(m_socketfd, level, optname, optval, optlen);
	}
}
