#include "NET_Core.hpp"

namespace NET 
{
	CMessageQueue::CMessageQueue()
		: m_mutex()
	{
		;
	}

	CMessageQueue::~CMessageQueue()
	{
		;
	}

	void CMessageQueue::pushMesssage(CBaseMessage* msg)
	{
		if ( msg != NULL ) {
			::std::lock_guard<::std::mutex> lock(m_mutex);
			
			m_msgQueue.push(msg);
		}
	}

	CBaseMessage* CMessageQueue::popMessage()
	{
		::std::lock_guard<::std::mutex> lock(m_mutex);
		
		if ( !m_msgQueue.empty() ) {
			CBaseMessage* msg = m_msgQueue.front();
			m_msgQueue.pop();
			
			return msg;
		} 
		return NULL;	
	}

	bool CMessageQueue::isEmpty()
	{
		::std::lock_guard<::std::mutex> lock(m_mutex);
		
		return m_msgQueue.empty();
	}
}
