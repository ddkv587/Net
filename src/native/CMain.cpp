#include "NET_Native.hpp"

#define CONFIG_PATH				"./config.xml"
#define CONFIG_PATH_BAK			"./.config.bak.xml"

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

	bool CMain::loadConfig()
	{
		::std::string strPath = CONFIG_PATH;
		FILE* fp = fopen(strPath, "r");
		if ( fp == NULL ) {
			strPath = CONFIG_PATH_BAK;
			fp = fopen(strPath, "r");
		}
		CHECK_R(fp != NULL, false);

		struct stat statBuff;
		CHECK_R( -1 != stat(strPath, &statBuff), false );


		int buffSize = statBuff.st_size + 1;
		char *pBuff = new char[buffSize];

		fread(pBuff, buffSize)

		rapidxml::xml_document<> doc;
		//load config




		//load module
		


	}

	void CMain::saveConfig()
	{
		;	
	}

	bool CMain::initialize()
	{
		if ( !loadConfig() ) return false;

		innerInitModule();
		m_bInitialized = true;
		return true;
	}

	void CMain::unInitialize()
	{
		innerRemoveModule();
		m_bInitialized = false;
	}

	void CMain::reload()
	{
		unInitialize();

		initialize();
	}

	void CMain::innerInitModule()
	{
		for ( CModule& module: m_lstModule ) {
			if ( module.isInitialized() ) continue;
			if ( !module.load() ) goto err;
			if ( !module.initialize() ) module.remove();	
err:
			LOG(WARNING) 
				<< CLog::format("load module %d failed", m_config.getConfig().strName);
		}
	}

	void CMain::mainLoop(void* arg)
	{
		int index = 0;
		while ( !m_bInitialized && index < 10 ) {
			m_bInitialized = initialize();
			++index;
		}
		LOG_IF(FATAL, index == 10) << "initialize failed! stop...";

		while (true) {
			
		}
	}
}
