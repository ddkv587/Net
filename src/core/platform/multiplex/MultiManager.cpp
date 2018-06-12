#include "NET_Core.hpp"

namespace NET
{
	CMultiManager::CMultiManager()
		: m_eType(EMT_NONE)
		, m_pBase(nullptr)
		, m_pEventLoop(nullptr)
	{
		;
	}

	CMultiManager::~CMultiManager()
	{
		destroy();
	}
	
	void CMultiManager::init(EMultiType type)
	{
		if ( nullptr != m_pBase ) delete m_pBase;

		switch( type )
		{
		case EMT_EPOLL:
			m_pBase = new CMultiEpoll();
			break;
		case EMT_KQUEUE:
			m_pBase = new CMultiKqueue();
			break;
		case EMT_SELECT:
			m_pBase = new CMultiSelect();
			break;
		case EMT_NONE:
			m_pBase = nullptr;
			return;
		}

		m_pBase->init();
	}

	void CMultiManager::destroy()
	{
		if ( nullptr != m_pBase ) {
			m_pBase->destroy();
			delete m_pBase;
			m_pBase = nullptr;
		}

		if ( nullptr != m_eventLoop )
		{
			if ( !m_eventLoop->lstFileEvent.empty() ) m_eventLoop->lstFileEvent.clear();
			if ( !m_eventLoop->lstTimeEvent.empty() ) m_eventLoop->lstTimeEvent.clear();
			if ( !m_eventLoop->lstFired.empty() ) m_eventLoop->lstFired.clear();
				
			delete m_eventLoop;
			m_eventLoop = nullptr;	
		}
	}

	INT CMultiManager::setSize(INT size)
	{
		CHECK_R(size <= SYSTEM_MAX_EVENTS, nullptr == m_eventLoop ? 0 : m_eventLoop->size);
		
		if ( nullptr == m_eventLoop ) {
			m_eventLoop = new EVENT_LOOP;
			CHECK_R( nullptr != m_eventLoop, 0 );

			m_eventLoop->maxfd = -1;
			m_eventLoop->lstFileEvent = ::std::vector<tagFileEvent>(size);
			m_eventLoop->lstTimeEvent = ::std::vector<tagTimeEvent>(size);
			m_eventLoop->lstFired = ::std::vector<tagFiredEvent>(size);

			if ( m_eventLoop->lstFileEvent.empty() || 
					m_eventLoop->lstTimeEvent.empty() ||
					m_eventLoop->lstFired.empty() ) {
				if ( !m_eventLoop->lstFileEvent.empty() ) m_eventLoop->lstFileEvent.clear();
				if ( !m_eventLoop->lstTimeEvent.empty() ) m_eventLoop->lstTimeEvent.clear();
				if ( !m_eventLoop->lstFired.empty() ) m_eventLoop->lstFired.clear();
				
				delete m_eventLoop;
				m_eventLoop = nullptr;	

				return 0;
			}
			m_eventLoop->size = size;

			//init new items
			for ( INT index=0; index < size; ++index ) 
				m_eventLoop->lstFileEvent[index].mask 	= NET_NONE;	
		} else {
			INT oldSize = m_eventLoop->size;
			CHECK_R(size > oldSize, oldSize);			
			
			m_eventLoop->lstFileEvent.resize(size);
			m_eventLoop->lstFired.resize(size);
			m_eventLoop->size = size;

			for ( INT index=oldSize; index < size; ++index ) 
				m_eventLoop->lstFileEvent[index].mask = NET_NONE;	
		}
		if ( nullptr != m_pBase ) m_pBase->setSize(size);

		return size;
	}

	INT CMultiManager::addFileEvent(INT fd, INT mask)
	{
		if ( nullptr != m_pBase ) return m_pBase->addFileEvent(fd, mask);
		return 0;
	}

	void CMultiManager::delFileEvent(INT fd, INT mask)
	{
		if ( nullptr != m_pBase ) m_pBase->delFileEvent(fd, mask);
	}

	INT CMultiManager::eventLoop(void* timeout)
	{
		if ( nullptr != m_pBase ) return m_pBase->eventLoop(timeout);
		return 0;
	}

	void CMultiManager::addTimer(const CTimer*)
	{
	
	}

	const CTimer* CMultiManager::getNearestTimer()
	{
		return nullptr;
	}
}
