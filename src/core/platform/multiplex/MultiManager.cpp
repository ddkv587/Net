#include "NET_Core.hpp"

namespace NET
{
	CMultiManager::CMultiManager()
		: m_uiSize(0)
		: m_eType(EMT_NONE)
		, m_uiBaseLine(0)
		, m_pBase(nullptr)
		, s_pEventLoop(nullptr)
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

		if ( nullptr != s_pEventLoop )
		{
			if ( !s_pEventLoop->lstFileEvent.empty() ) s_pEventLoop->lstFileEvent.clear();
			if ( !s_pEventLoop->lstTimeEvent.empty() ) s_pEventLoop->lstTimeEvent.clear();
			if ( !s_pEventLoop->lstFired.empty() ) s_pEventLoop->lstFired.clear();
				
			delete s_pEventLoop;
			s_pEventLoop = nullptr;	
		}
	}

	UINT CMultiManager::setSize(UINT size)
	{
		if ( nullptr != m_pBase && size != m_uiSize ) 
			m_uiSize = m_pBase->setSize(size);

		return m_uiSize;
	}

	INT CMultiManager::addFileEvent(INT fd, INT mask, fileProc *proc, DATA* clientData)
	{
		if ( nullptr != m_pBase ) 
			CHECK_R( 0 == m_pBase->addFileEvent(fd, mask, s_pEventLoop)), -1 );

		FILE_EVENT* event = s_pEventLoop[fd];
		CHECK_R(nullptr != event, -1);
		event->mask |= mask;
		if ( mask & NET_READABLE ) event->readProc = proc;
		if ( mask & NET_WRITEABLE ) event->writeProc = proc;
		event->clientData = clientData;

		return 0;
	}

	void CMultiManager::delFileEvent(INT fd, INT mask)
	{
		if ( nullptr != m_pBase ) 
			m_pBase->delFileEvent(fd, mask, s_pEventLoop);

		FILE_EVENT* event = s_pEventLoop->lstFileEvent;
		fe->mask = fe->mask & (~mask);
	}

	INT CMultiManager::eventLoop(void* timeout)
	{
		if ( nullptr != m_pBase ) 
			return m_pBase->eventLoop(timeout, s_pEventLoop);
		return 0;
	}

	void CMultiManager::addTimer(const CTimer*)
	{
	
	}

	UINT CMultiManager::setEventLoopSize(UINT size)
	{
		::std::lock_guard<::std::mutex> lock(m_mutex);

		if ( nullptr == s_pEventLoop ) {
			s_pEventLoop = new EVENT_LOOP;
			CHECK_R( nullptr != s_pEventLoop, 0 );

			s_pEventLoop->lstFileEvent 	= ::std::vector<tagFileEvent>(size);
			s_pEventLoop->lstFired 		= ::std::vector<tagFiredEvent>(size + MAX_TIME_EVENT_SIZE);

			s_pEventLoop->lstTimeEvent 	= ::std::vector<tagTimeEvent>(MAX_TIME_EVENT_SIZE);
			s_pEventLoop->size = size;

			//init new items
			for ( INT index=0; index < size; ++index ) 
				s_pEventLoop->lstFileEvent[index].mask 	= NET_NONE;	

		} else {
			INT oldSize = s_pEventLoop->size;
			CHECK_R( size > oldSize, oldSize );			
			
			s_pEventLoop->lstFileEvent.resize(size);
			s_pEventLoop->lstFired.resize(size);
			s_pEventLoop->size = size;

			for ( INT index=oldSize; index < size; ++index ) 
				s_pEventLoop->lstFileEvent[index].mask = NET_NONE;
		}
	}

	void CMultiManager::enableEdgeTrigger(BOOLEAN on)
	{
		if ( nullptr != m_pBase && m_eType == EMT_EPOLL ) {
			CMultiEpoll* poll = static_cast<CMultiEpoll*>(m_pBase);

			poll->enableEdgeTrigger(on);
		}
	}

	const CTimer* CMultiManager::getNearestTimer()
	{
		return nullptr;
	}
}
