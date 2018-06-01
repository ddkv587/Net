#include "NET_Core.hpp"

namespace NET 
{
	CTimer::CTimer()
		: m_uiInterval(0)
		, m_uiRepeat(0)
		, m_bIsRunning(FALSE)
	{
		;
	}

	CTimer::CTimer(UINT uiInterval, INT iRepeat)
		: m_uiInterval(uiInterval)
		, m_iRepeat(iRepeat)
		, m_bIsRunning(FALSE)
	{
		;
	}

	CTimer::~CTimer()
	{
		if ( m_bIsRunning ) 
			stop();
	}

	void CTimer::start()
	{
		;
	}

	void CTimer::stop()
	{
		;
	}

	void CTimer::reset()
	{
		;
	}

	void CTimer::addListener(const ITimerListener* pListener)
	{
		::std::lock_guard<::std::mutex> guard(m_mutex);
		for ( INT i=0; i < m_lstListener.size(); ++i ) {
			if ( m_lstListener[i] == pListener ) return;
		}
		m_lstListener.push_back(pListener);
	}

	void CTimer::delListener(const ITimerListener* pListener)
	{
		::std::lock_guard<::std::mutex> guard(m_mutex);
		for ( INT i=0; i < m_lstListener.size(); ++i ) {
			if ( m_lstListener[i] == pListener ) {
				m_lstListener.erase(i);
				return;
			}
		}
	}
}
