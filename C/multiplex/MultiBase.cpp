#include "Multiplex.hpp"

namespace NET
{
	
	CMultiBase::CMultiBase()
		: m_eType(EMT_NONE)
		, m_pEventLoop(NULL)
	{
		
	}

	CMultiBase::~CMultiBase()
	{
		if ( NULL != m_pEventLoop ) {
			if ( !m_pEventLoop->event.empty() ) m_pEventLoop->event.clear();
			if ( !m_pEventLoop->fired.empty() ) m_pEventLoop->fired.clear();
			
			delete m_pEventLoop;
			m_pEventLoop = NULL;
		}
	}

	int CMultiBase::setSize(int size)
	{
		CHECK_R(size <= SYSTEM_MAX_EVENTS, NULL == m_pEventLoop ? 0 : m_pEventLoop->size);
		
		if ( NULL == m_pEventLoop ) {
			m_pEventLoop = new EVENT_LOOP;
			CHECK_R( NULL != m_pEventLoop, 0 );

			m_pEventLoop->maxfd = -1;
			m_pEventLoop->event = ::std::vector<tagFileEvent>(size);
			m_pEventLoop->fired = ::std::vector<tagFiredEvent>(size);

			if ( m_pEventLoop->event.empty() || m_pEventLoop->fired.empty() ) {
				if ( !m_pEventLoop->event.empty() ) m_pEventLoop->event.clear();
				if ( !m_pEventLoop->fired.empty() ) m_pEventLoop->fired.clear();
				
				delete m_pEventLoop;
				m_pEventLoop = NULL;	

				return 0;
			}
			m_pEventLoop->size = size;
		} else {
			CHECK_R(size > m_pEventLoop->size, m_pEventLoop->size);			
			
			m_pEventLoop->event.resize(size);
			m_pEventLoop->fired.resize(size);

			m_pEventLoop->size = size;
		}
		return m_pEventLoop->size;
	}

	void CMultiBase::addFileEvent(int fd, int mask)
	{
		;
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
