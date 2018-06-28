#include "NET_Core.hpp"

namespace NET
{
    void CFileEvent::clearBuff()
    {
        if ( nullptr != m_dataBuff ) {
            delete (CHAR*)m_dataBuff;
            m_dataBuff = nullptr;
            m_dataSize = 0;
        }
    }

    UINT CFileEvent::resize(UINT uiSize)
    {
        if ( uiSize)
    }
} //NET