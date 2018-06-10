#include "NET_Core.hpp"

namespace NET
{
	
	CMultiBase::CMultiBase()
		: m_eType(EMT_NONE)
		, m_eventLoop(NULL)
		, m_bIsEdgeTrigger(false)
	{
		;
	}

	CMultiBase::~CMultiBase()
	{
		if ( NULL != m_eventLoop ) {
			if ( !m_eventLoop->event.empty() ) m_eventLoop->event.clear();
			if ( !m_eventLoop->fired.empty() ) m_eventLoop->fired.clear();
			
			delete m_eventLoop;
			m_eventLoop = NULL;
		}
	}

	int CMultiBase::setSize(int size)
	{
		CHECK_R(size <= SYSTEM_MAX_EVENTS, NULL == m_eventLoop ? 0 : m_eventLoop->size);
		
		if ( NULL == m_eventLoop ) {
			m_eventLoop = new EVENT_LOOP;
			CHECK_R( NULL != m_eventLoop, 0 );

			m_eventLoop->maxfd = -1;
			m_eventLoop->event = ::std::vector<tagFileEvent>(size);
			m_eventLoop->fired = ::std::vector<tagFiredEvent>(size);

			if ( m_eventLoop->event.empty() || m_eventLoop->fired.empty() ) {
				if ( !m_eventLoop->event.empty() ) m_eventLoop->event.clear();
				if ( !m_eventLoop->fired.empty() ) m_eventLoop->fired.clear();
				
				delete m_eventLoop;
				m_eventLoop = NULL;	

				return 0;
			}
			m_eventLoop->size = size;

			for ( int index=0; index < size; ++index ) {
				m_eventLoop->event[index].fd = index;	
				m_eventLoop->event[index].mask = NET_NONE;	
			}
		} else {
			int oldSize = m_eventLoop->size;
			CHECK_R(size > oldSize, oldSize);			
			
			m_eventLoop->event.resize(size);
			m_eventLoop->fired.resize(size);

			m_eventLoop->size = size;

			for ( int index=oldSize; index < size; ++index ) {
				m_eventLoop->event[index].fd = index;	
				m_eventLoop->event[index].mask = NET_NONE;	
			}
		}
		return m_eventLoop->size;
	}

	int CMultiBase::addFileEvent(int fd, int mask)
	{
		return 0;
	}

	void CMultiBase::delFileEvent(int fd, int mask)
	{
		;
	}

	int CMultiBase::eventLoop(void* timeout)
	{
		return 0;
	}
}
