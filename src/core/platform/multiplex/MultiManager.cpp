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
		if ( nullptr != m_pBase ) 
		{ 
			m_pBase->destroy();
		}

		switch( type )
		{	
#ifdef OS_BSD
		case EMT_KQUEUE:
			m_pBase = new CMultiKqueue();
			break;
#else
		case EMT_EPOLL:
			m_pBase = new CMultiEpoll();
			break;
		case EMT_SELECT:
			m_pBase = new CMultiSelect();
			break;
#endif
		case EMT_NONE:
		default:
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

		if ( nullptr != m_pEventLoop )
		{
			if ( !m_pEventLoop->lstFileEvent.empty() ) m_pEventLoop->lstFileEvent.clear();
			if ( !m_pEventLoop->lstTimeEvent.empty() ) m_pEventLoop->lstTimeEvent.clear();
			if ( !m_pEventLoop->lstFired.empty() ) m_pEventLoop->lstFired.clear();
				
			delete m_pEventLoop;
			m_pEventLoop = nullptr;	
		}
	}

	INT CMultiManager::setSize(INT size)
	{
		CHECK_R(size <= SYSTEM_MAX_EVENTS, nullptr == m_pEventLoop ? 0 : m_pEventLoop->size);
		
		if ( nullptr == m_pEventLoop ) {
			m_pEventLoop = new EVENT_LOOP;
			CHECK_R( nullptr != m_pEventLoop, 0 );

			m_pEventLoop->maxfd = -1;
			m_pEventLoop->lstFileEvent = ::std::vector<tagFileEvent>(size);
			m_pEventLoop->lstTimeEvent = ::std::vector<tagTimeEvent>(size);
			m_pEventLoop->lstFired = ::std::vector<tagFiredEvent>(size);

			if ( m_pEventLoop->lstFileEvent.empty() || 
					m_pEventLoop->lstTimeEvent.empty() ||
					m_pEventLoop->lstFired.empty() ) {
				if ( !m_pEventLoop->lstFileEvent.empty() ) m_pEventLoop->lstFileEvent.clear();
				if ( !m_pEventLoop->lstTimeEvent.empty() ) m_pEventLoop->lstTimeEvent.clear();
				if ( !m_pEventLoop->lstFired.empty() ) m_pEventLoop->lstFired.clear();
				
				delete m_pEventLoop;
				m_pEventLoop = nullptr;	

				return 0;
			}
			m_pEventLoop->size = size;

			//init new items
			for ( INT index=0; index < size; ++index ) 
				m_pEventLoop->lstFileEvent[index].mask 	= NET_NONE;	
		} else {
			INT oldSize = m_pEventLoop->size;
			CHECK_R(size > oldSize, oldSize);			
			
			m_pEventLoop->lstFileEvent.resize(size);
			m_pEventLoop->lstFired.resize(size);
			m_pEventLoop->size = size;

			for ( INT index=oldSize; index < size; ++index ) 
				m_pEventLoop->lstFileEvent[index].mask = NET_NONE;	
		}
		if ( nullptr != m_pBase ) m_pBase->setSize(size);

		return size;
	}

	void CMultiManager::enableEdgeTrigger(BOOLEAN on)
	{
		if ( nullptr != m_pBase && m_eType == EMT_EPOLL ) {
			CMultiEpoll* poll = static_cast<CMultiEpoll*>(m_pBase);

			poll->enableEdgeTrigger(on);
		}
	}

	INT CMultiManager::addFileEvent(INT fd, INT mask)
	{
		if ( nullptr != m_pBase ) return m_pBase->addFileEvent(fd, mask, m_pEventLoop);
		return -1;
	}

	void CMultiManager::delFileEvent(INT fd, INT mask)
	{
		if ( nullptr != m_pBase ) m_pBase->delFileEvent(fd, mask, m_pEventLoop);
	}

	INT CMultiManager::eventLoop(void* timeout)
	{
		if ( nullptr != m_pBase ) return m_pBase->eventLoop(timeout, m_pEventLoop);
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
