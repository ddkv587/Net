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
		for ( INT i=0; i < m_lstListener.size(); ++i ) {
			if ( m_lstListener[i] == pListener ) return;
		}
		m_lstListener.push_back(pListener);
	}

	void CTimer::delListener(const ITimerListener* pListener) noexcept
	{
		::std::lock_guard<::std::mutex> guard(m_mutex);
		for ( INT i=0; i < m_lstListener.size(); ++i ) {
			if ( m_lstListener[i] == pListener ) {
				m_lstListener.erase(m_lstListener.begin() + i);
				return;
			}
		}
	}
}
