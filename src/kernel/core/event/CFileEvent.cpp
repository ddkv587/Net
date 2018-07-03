#include "NET_Core.hpp"

namespace NET
{
    CFileEvent::CFileEvent(UINT uiPriority)
    {
        setPriority(1000);
    }
    
    virtual CFileEvent::~CFileEvent()
    {
        ;
    }
            
    BOOLEAN CFileEvent::readProc()
    {
        ;
    }

    BOOLEAN CFileEvent::writeProc()
    {
        ;
    }

    void CFileEvent::release()
    {
        if ( nullptr != m_dataBuffer ) {
            delete (CHAR*)m_dataBuffer;

            m_uiSize =0;
            m_uiPoint = 0;
        }
    }
    
    void CFileEvent::clear()
    {
        if ( nullptr != m_dataBuffer && m_uiPoint != 0 ) {
            memset(m_dataBuffer, 0, m_uiPoint);

            m_uiPoint = 0;
        }
    }

    UINT CFileEvent::resize(UINT uiSize)
    {
        if ( uiSize > m_uiSize ) {
            CHAR* buff = (CHAR*)m_dataBuffer;

            m_dataBuffer = new CHAR[uiSize];
            memccpy(m_dataBuffer, buff, m_uiPoint);
            m_uiSize = uiSize;

            delete buff;
        }
    }  

    void CFileEvent::setNonBlock()
	{
		if ( m_fd > 0 ) {
            INT flags = fcntl(fd, F_GETFL, 0);

		    if ( flags == -1 ) 
			    handle_error("fcntl get");

		    flags |= O_NONBLOCK;

		    if ( fcntl( fd, F_SETFL, flags ) == -1 ) 
			    handle_error("fcntl set");
        }		
	}
} //NET