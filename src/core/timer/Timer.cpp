#include "NET_Core.hpp"

namespace NET 
{
	CTimer::CTimer() noexcept
		: m_uiInterval(0)
		, m_iRepeat(0)
		, m_bIsRunning(FALSE)
	{
		;
	}

	CTimer::CTimer(UINT uiInterval, INT iRepeat) noexcept
		: m_uiInterval(uiInterval)
		, m_iRepeat(iRepeat)
		, m_bIsRunning(FALSE)
	{
		;
	}

	CTimer::~CTimer() noexcept
	{
		if ( m_bIsRunning ) 
			stop();
	}

	void CTimer::start() noexcept
	{
		;
	}

	void CTimer::stop() noexcept
	{
		;
	}

	void CTimer::reset() noexcept
	{
		;
	}

	void CTimer::addListener(ITimerListener* pListener) noexcept
	{
		::std::lock_guard<::std::mutex> guard(m_mutex);
		for ( UINT ui=0; ui < m_lstListener.size(); ++ui ) {
			if ( m_lstListener[ui] == pListener ) return;
		}
		m_lstListener.push_back(pListener);
	}

	void CTimer::delListener(const ITimerListener* pListener) noexcept
	{
		::std::lock_guard<::std::mutex> guard(m_mutex);
		for ( UINT ui=0; ui < m_lstListener.size(); ++ui ) {
			if ( m_lstListener[ui] == pListener ) {
				m_lstListener.erase(m_lstListener.begin() + ui);
				return;
			}
		}
	}
}
