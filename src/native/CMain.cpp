#include "NET_Native.hpp"

namespace NET 
{
	CMain* CMain::s_pInstance = NULL;
	
	CMain::CMain()
		: m_bInitialized(false)
	{
		;
	}

	CMain::~CMain()
	{
		;
	}

	CMain* CMain::getInstance()
	{
		if ( NULL == s_pInstance ) {
			s_pInstance = new CMain();
		}
		return s_pInstance;
	}

	BOOLEAN CMain::initialize()
	{
        CHECK_R( ConfigParser::getInstance()->initialize(), FALSE);
        if ( !innerInitListener() ) goto err_listener;
        if ( !innerInitProcessor() ) goto err_processor;
        if ( !innerInitUpdate() ) goto err_update();
        
		m_bInitialized = true;
		return true;

    err_update:
        innerDestroyProcessor();
    err_processor:
        innerDestroyListener();
    err_listener:
        
        return FALSE;
	}

	void CMain::unInitialize()
	{
        innerDestroyUpdate();
        innerDestroyProcessor();
        innerDestroyListener();
		m_bInitialized = false;
	}

	void CMain::mainLoop(void* arg)
	{
		int index = 0;

		while ( !m_bInitialized && index < 10 ) {
			m_bInitialized = initialize();
			++index;
		}
		LOG_IF(FATAL, index == 10) << "initialize failed! stop...";
        
        //thread start
        ::std::list<CProcessor*>::iterator itor = m_lstProcessor.begin();
        for(; itor != m_lstProcessor.end(); ++itor ) {
            (*itor)->run();
        }
        m_pListener->run();

		while (true) {
            ;       //main loop just sleep now;
		}
	}
    
    BOOLEAN CMain::innerInitListener()
    {
        m_pListener = new CListener();
        
        tagSocketInfo& info = ConfigParser::getInstance()->getSocketInfo();
        m_pListener->getSocketServer()->setPort( info.uiPort );
        m_pListener->getSocketServer()->setKeepAlive( info.bKeepAlive, info.uiAliveValue );
        m_pListener->getSocketServer()->setTimeOut( info.uiTimeOut );
        if ( info.bReusePort )
            m_pListener->getSocketServer()->setReusePort( true );
        else
            m_pListener->getSocketServer()->setReuseAddress( true );

        return TRUE;
    }
    
    BOOLEAN CMain::innerInitProcessor()
    {
        tagSystemInfo& info = ConfigParser::getInstance()->getSystemInfo();
        
        for ( UINT uiIndex=0; uiIndex < info.m_uiThreadCount - 1; ++uiIndex ) {
            
            CProcessor* processor = new CProcessor();
            
            m_lstProcessor.push_back(processor);
            if ( NULL != m_lstListener ) m_lstListener->addFileListener(processor);
        }
        
        return TRUE;
    }
    
    BOOLEAN CMain::innerInitUpdate()
    {
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
        ::std::list<CProcessor*>::iterator itor = m_lstProcessor.begin();
        for(; itor != m_lstProcessor.end(); ++itor ) {
            (*itor)->stop();
            delete (*itor);
        }
        m_lstProcessor.clear();
    }
    
    void CMain::innerDestroyUpdate()
    {
        ;
    }
}
