#include "NET_Core.hpp"

namespace NET 
{
	#defien DEFAULT_THREAD_COUNT	10;

	CConfig* CConfig::m_pInstance = NULL;

	CConfig* CConfig::getInstance()
	{
		if ( NULL == m_pInstance ) {
			m_pInstance = new CConfig();
		}
		return m_pInstance;
	}

	CConfig::CConfig()
	{
		m_tagSysInfo.m_uiPriority 		= 0;
		m_tagSysInfo.m_uiThreadCount 	= 10;
	}

	CConfig::~CConfig()
	{
		;
	}

	void CConfig::initialize()
	{
		buildSystenInfo();
	}

	void CConfig::buildSystenInfo()
	{
		m_tagSysInfo.m_uiThreadCount = 0;

		unsigned int hardware = ::std::thread::hardware_concurrency();
		m_tagSysInfo.m_uiPriority = ::std::min(hardware != 0 ? hardware : DEFAULT_THREAD_COUNT, m_tagSysInfo.m_uiThreadCount);
	}
}
