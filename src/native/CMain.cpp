#include "NET_Native.hpp"

namespace NET 
{
	CMain* CMain::s_pInstance = NULL;

	CMain* CMain::getInstance()
	{
		if ( NULL == s_pInstance ) {
			s_pInstance = new CMain();
		}
		return s_pInstance;
	}
	
	CMain::CMain()
		: m_pUpdate(NULL)
		, m_pListener(NULL)
		, m_bInitialized(FALSE)
	{
		;
	}

	CMain::~CMain()
	{
		;
	}

	BOOLEAN CMain::initialize()
	{
		LOG(INFO) << "initialize start...";

        CHECK_R( ConfigParser::getInstance()->initialize(), FALSE);
        if ( !innerInitListener() ) goto err_listener;
        if ( !innerInitProcessor() ) goto err_processor;
        if ( !innerInitUpdate() ) goto err_update;
        
		m_bInitialized = TRUE;
		return TRUE;

    err_update:
		LOG(WARNING) << CLog::format(" initialize update error! \n");
        innerDestroyProcessor();
    err_processor:
		LOG(WARNING) << CLog::format(" initialize processor error! \n");
        innerDestroyListener();
    err_listener:
		LOG(WARNING) << CLog::format(" initialize listener error! \n");
        
        return FALSE;
	}

	void CMain::unInitialize()
	{
        innerDestroyUpdate();
        innerDestroyProcessor();
        innerDestroyListener();
		m_bInitialized = false;
	}

	void CMain::start(void* arg)
	{
		UINT uiIndex = 0;
		LOG(INFO) << "start demo....";

		while ( !m_bInitialized && uiIndex < 10 ) {
			m_bInitialized = initialize();
			++uiIndex;
		}
		LOG_IF(FATAL, uiIndex == 10) << "initialize failed! stop...";
        
        //run procrssor thread
        ::std::list<CProcessor*>::iterator itor = m_lstShortTurn.begin();
        for(; itor != m_lstShortTurn.end(); ++itor ) {
            (*itor)->run();
        }
        m_pListener->run();

		LOG(INFO) << "initialize end, start main loop....";
		while (true) {
            ;       //main loop just sleep now;
			::std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		unInitialize();
	}
    
    BOOLEAN CMain::innerInitListener()
    {
		LOG(INFO) << "begin to initialize listener";

        m_pListener = new CListener();
        
		const ConfigParser::tagSocketInfo& info = ConfigParser::getInstance()->getSocketInfo();
        
        m_pListener->server()->setPort( info.uiPort );
        m_pListener->server()->setKeepAlive( info.bKeepAlive, info.uiAliveValue );
        m_pListener->server()->setTimeOut( info.uiTimeOut );
        if ( info.bReusePort )
            m_pListener->server()->setReusePort( true );
        else
            m_pListener->server()->setReuseAddress( true );

        return TRUE;
    }
    
    BOOLEAN CMain::innerInitProcessor()
    {
		LOG(INFO) << "begin to initialize processor";

		// init two type processor: short turn or long turn
		const ConfigParser::tagSystemInfo& info = ConfigParser::getInstance()->getSystemInfo();
        
        for ( INT i=0; i < info.uiThreadCount - 1; ++i ) {
            CProcessor* processor = new CProcessor();
            
            m_lstShortTurn.push_back(processor);
            if ( NULL != m_pListener ) m_pListener->addListener(processor);
        }
        
        return TRUE;
    }
    
    BOOLEAN CMain::innerInitUpdate()
    {
		LOG(INFO) << "begin to initialize update";

        return TRUE;
    }
    
    void CMain::innerDestroyListener()
    {
        if ( NULL != m_pListener ) {
            m_pListener->stop();
           
            delete m_pListener;
            m_pListener = NULL;
        }
    }
    
    void CMain::innerDestroyProcessor()
    {
        ::std::list<CProcessor*>::iterator itor = m_lstShortTurn.begin();
        for(; itor != m_lstShortTurn.end(); ++itor ) {
            (*itor)->stop();
            delete (*itor);
        }
        m_lstShortTurn.clear();

		itor = m_lstLongTurn.begin();
        for(; itor != m_lstLongTurn.end(); ++itor ) {
            (*itor)->stop();
            delete (*itor);
        }
        m_lstLongTurn.clear();
    }
    
    void CMain::innerDestroyUpdate()
    {
        ;
    }
}
