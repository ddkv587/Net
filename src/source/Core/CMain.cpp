namespace NET
{
    CMain::CMain* CMain::s_pInstance = NULL;

    CMain* CMain::getInstance()
    {
        if ( !s_pInstance )
        {
            s_pInstance = new CMain();
        }

        return s_pInstance;
    }

    void CMain::delInstance()
    {
        if ( s_pInstance )
        {
            delete s_pInstance;
            s_pInstance = NULL;
        }
    }

    CMain::CMain()
        : m_bInitialized( FALSE )
    {
        initialize();

        threadName( "CMain" );
    }

    CMain::~CMain()
    {
        if ( m_bInitialized )
        {
            uninitialize();
        }
    }

    void CMain::initialize()
    {
        // TODO
        // add initialize code

        m_bInitialized = TRUE;
    }

    void CMain::uninitialize()
    {
        // TODO
        // add uninitialize code

        m_bInitialized = FALSE;
    }

    void CMain::pushMessage( CBaseMessage* pMsg )
    {
        switch ( pMsg->type() )
        {
            case MT_UNKNOW:
            // do nothing
            break;

            case MT_SYSTEM:
                m_sysMessage->push( pMsg );
            break;

            case MT_USER:
                m_userMessage->push( pMsg );
            break;

            case MT_TIMER;
                m_timerMessage->push( pMsg );
            break;
        }
    }

    CBaseMessage* CMain::popMessage()
    {
        if ( m_sysMessage.size() > 0 ) {
            return m_sysMessage.pop();
        }

        if ( m_timerMessage.size() > 0 ) {
            return m_sysMessage.pop();
        }

        if ( m_userMessage.size() > 0 ) {
            return m_sysMessage.pop();
        }

        return NULL;
    }

    void CMain::mainLoop( void* arg )
    {
        while ( !m_bStop )
        {
            CBaseMessage* pMsg = popMessage();

            if ( pMsg ) {
                pMsg->handleMessage();

                delete pMsg;

                s_waitTime = ::std::max( s_waitTime >> 2, 0xFF );
            }
        }
    }
}