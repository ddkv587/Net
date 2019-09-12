#include "OpenHMI_WindowSystem.h"

namespace OpenHMI
{
    CMsgScreenTouch::CMsgScreenTouch(
                    IScreenTouchListener    *pListener,
                    const CCursor            &cursor)
        : m_pListener(pListener)
        , m_cursor(cursor)
    {
        m_eType = CBaseMessage::MT_SCREEN_TOUCH;
        m_uiID = m_eType;
    }

    CMsgScreenTouch::~CMsgScreenTouch()
    {
        ;
    }

    void CMsgScreenTouch::handleMessage()
    {
        if (m_pListener == NULL)
        {
            return;
        }

        switch (m_cursor.getTouchStatus())
        {
        case CCursor::TS_ADD:
            m_pListener->onCursorAdded(m_cursor);
            break;

        case CCursor::TS_UPDATE:
            m_pListener->onCursorUpdated(m_cursor);
            break;

        case CCursor::TS_REMOVE:
            m_pListener->onCursorRemoved(m_cursor);
            break;

        case CCursor::TS_UNDEFINE:
            break;
        }
    }
}