#include "NET_Core.hpp"

namespace NET
{
    CSocketBase::CSocketBase()
        : m_fd(-1)
        , m_bIsOpenning(FALSE)
    {
        LOG_IF( ERROR, -1 == ( m_fd = socket(AF_INET, SOCK_STREAM, 0) ) )
            << CLog::format( "[%s, %d]  set socket to keep alive error: %s" ,__FILE__, __LINE__, strerror(errno) );
    }
    
    CSocketBase::~CSocketBase()
    {
        destroy();
    }

    void CSocketBase::init()
    {
        LOG_IF( ERROR, -1 == ( m_fd = socket(AF_INET, SOCK_STREAM, 0) ) )  
            << CLog::format( "[%s, %d]  set socket to keep alive error: %s" ,__FILE__, __LINE__, strerror(errno) );
        m_bIsOpenning = TRUE;
    }

    void CSocketBase::destroy()
    {
        if ( m_bIsOpenning ) {
            close( m_fd );
            m_bIsOpenning = FALSE;
        }
    }

    void CSocketBase::setKeepAlive( BOOLEAN on, INT interval )
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
    
    void CSocketBase::setTimeOut( INT timeout )
    {
#ifndef OS_BSD
        LOG_IF( ERROR, -1 == setsockopt(m_fd, IPPROTO_TCP, TCP_USER_TIMEOUT, &timeout, sizeof(timeout)) )
                << CLog::format( "[%s, %d]  set socket timeout error: %s" ,__FILE__, __LINE__, strerror(errno) );    
#endif
    }
    
    void CSocketBase::setReusePort( BOOLEAN on )
    {
        INT val = on ? 1 : 0;

        LOG_IF( ERROR, -1 == setsockopt(m_fd, SOL_SOCKET, SO_REUSEPORT, &val, sizeof(val)) )
                   << CLog::format( "[%s, %d]  set socket reuse port error: %s" ,__FILE__, __LINE__, strerror(errno) );
    }

    void CSocketBase::setReuseAddress( BOOLEAN on )
    {
        INT val = on ? 1 : 0;

        LOG_IF( ERROR, -1 == setsockopt(m_fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val)) )
                << CLog::format( "[%s, %d]  set socket reuse addr error: %s" ,__FILE__, __LINE__, strerror(errno) );    
    }
    
    void CSocketBase::setNonBlock( BOOLEAN on )
    {
        INT flags = fcntl(m_fd, F_GETFL, 0);
        INT ret = on ? fcntl(m_fd, F_SETFL, flags | O_NONBLOCK) : fcntl(m_fd, F_SETFL, flags & ~O_NONBLOCK);

        LOG_IF( ERROR, -1 == ret )
                << CLog::format( "[%s, %d]  set socket O_NONBLOCK error: %s" ,__FILE__, __LINE__, strerror(errno) );
    }

    void CSocketBase::setLinger( BOOLEAN on, INT val )
    {
          struct linger li;
          li.l_onoff = on ? 1 : 0;
          li.l_linger = val; 

        LOG_IF( ERROR, -1 == setsockopt(m_fd, SOL_SOCKET, SO_LINGER, &li, sizeof(li)) )
                << CLog::format( "[%s, %d]  set socket SO_LINGER error: %s" ,__FILE__, __LINE__, strerror(errno) );
    }

    void CSocketBase::setNoDelay( BOOLEAN on )
    {
          INT val = on ? 1 : 0;

        LOG_IF( ERROR, -1 == setsockopt(m_fd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val)) )
                << CLog::format( "[%s, %d]  set socket TCP_NODELAY error: %s" ,__FILE__, __LINE__, strerror(errno) );
    }

    void CSocketBase::setSendBuffSize( INT64 size )
    {
        LOG_IF( ERROR, -1 == setsockopt(m_fd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size)) )
                << CLog::format( "[%s, %d]  set socket SO_SNDBUF error: %s" ,__FILE__, __LINE__, strerror(errno) );
    }

    void CSocketBase::setRecvBuffSize( INT64 size )
    {
        LOG_IF( ERROR, -1 == setsockopt(m_fd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size)) )
                << CLog::format( "[%s, %d]  set socket SO_RCVBUF error: %s" ,__FILE__, __LINE__, strerror(errno) );
    }

    // CSocketBase& operator>>(const CSocketBase& socket, const CBuffer* const pBuffer)    
    // {
    //     //send
    //     UINT uiSize = 0;
    //     CHAR buffer[1024];
    //     while ( TRUE )
    //     {
    //         INT ret = recv(socket.m_fd, buffer, 1024);
    //         if ( ret == -1 ) {
    //             switch(errno) 
    //             {
    //             case EAGAIN:
    //             case EWOULDBLOCK:
    //                 return socket;
    //             default:
    //                 throw SocketException(errno);
    //                 return socket;
    //             }
    //         }
    //         pBuffer->writeChar();
    //         uiSize += ret;    
    //     }

    //     return socket;
    // }

    // CSocketBase& operator<<(const CSocketBase& socket, CBuffer* const pBuffer)
    // {
    //     //recive
    //     ;
    // }
}
