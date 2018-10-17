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
		: m_bInitialized(FALSE)
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
        CHECK_R( innerInitSystem(), FALSE );

        if ( !innerInitListener() ) {
            LOG(WARNING) << CLog::format(" initialize listener error! \n");
            return FALSE;
        }
        if ( !innerInitProcessor() ) {
            LOG(WARNING) << CLog::format(" initialize processor error! \n");
            goto err_processor;
        }
        if ( !innerInitUpdate() ) {
            LOG(WARNING) << CLog::format(" initialize update error! \n");     
            goto err_update;
        }
        
		m_bInitialized = TRUE;
		return TRUE;

    err_update:
		innerDestroyProcessor();
    err_processor:	
        innerDestroyListener();

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
		LOG(INFO) << "start demo....";

        // initialize for 10 times
		for ( UINT uiIndex = 0; !m_bInitialized && uiIndex < 10; ++uiIndex ) {
			m_bInitialized = initialize();
		}
		LOG_IF( FATAL, m_bInitialized == FALSE ) << "initialize failed! stop...";
        
        //run procrssor thread
        ::std::list<CWorker*>::iterator itor = m_lstWorker.begin();
        for(; itor != m_lstWorker.end(); ++itor ) {
            (*itor)->run();
        }

        //run listener thread
        m_pListener->run();

		LOG(INFO) << "initialize end, start main loop....";
		while (true) {
            //main loop just sleep now;
			::std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}

		unInitialize();
	}

    BOOLEAN CMain::innerInitSystem()
    {
        LOG(INFO) << "begin to initialize system";  
        const ConfigParser::tagSystemInfo& info = ConfigParser::getInstance()->getSystemInfo();
        
        CMD::SYSCTL_MAX_FILE(65535);
        CMD::LIMIT_OPEN_FILE(info.uiMaxFileSize);
        CMultiManager::setEventLoopSize(info.uiMaxFileSize);

        return TRUE;
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
            m_pListener->server()->setReusePort( TRUE );
        else
            m_pListener->server()->setReuseAddress( TRUE );

        return TRUE;
    }
    
    BOOLEAN CMain::innerInitProcessor()
    {
		LOG(INFO) << "begin to initialize processor";

		// init two type processor: short turn or long turn
		const ConfigParser::tagSystemInfo& info = ConfigParser::getInstance()->getSystemInfo();
        
        for ( INT i=0; i < info.uiThreadCount - 1; ++i ) {      // one is listener
            LOG(INFO) << CLog::format("create new worker: %d", i); 
            CWorker* worker = new CWorker();
            worker->setMaxSize(info.uiMaxFileSize / (info.uiThreadCount - 1) );

            m_lstWorker.push_back(worker);
            if ( NULL != m_pListener ) m_pListener->addListener(worker);
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
        ::std::list<CWorker*>::iterator itor = m_lstWorker.begin();
        for(; itor != m_lstWorker.end(); ++itor ) {
            (*itor)->stop();
            delete (*itor);
        }
        m_lstWorker.clear();
    }
    
    void CMain::innerDestroyUpdate()
    {
        ;
    }
}
