#include "NET_Core.hpp"

namespace NET
{
	::std::mutex 	CMultiManager::m_mutex;	
	EVENT_LOOP* 	CMultiManager::s_pEventLoop = nullptr;		

	CMultiManager::CMultiManager()
		: m_uiSize(0)
		, m_eType(EMT_NONE)
		, m_pBase(nullptr)
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
				
			delete s_pEventLoop;
			s_pEventLoop = nullptr;	
		}

		if ( !m_lstFired.empty() ) m_lstFired.clear();		
	}

	UINT CMultiManager::setSize(UINT size)
	{
		if ( nullptr != m_pBase && size != m_uiSize ) 
			m_uiSize = m_pBase->setSize(size);

		return m_uiSize;
	}

	INT CMultiManager::addFileEvent(INT fd, INT mask, fileProc proc, DATA* clientData)
	{
		CHECK_R( UINT(fd) < s_pEventLoop->uiSize, -1);
		if ( nullptr != m_pBase ) 
			CHECK_R( 0 == m_pBase->addFileEvent(fd, mask, s_pEventLoop), -1 );

		FILE_EVENT& event = s_pEventLoop->lstFileEvent[fd];
		event.mask |= mask;
		if ( mask & NET_READABLE ) event.readProc = proc;
		if ( mask & NET_WRITEABLE ) event.writeProc = proc;
		event.clientData = clientData;

		return 0;
	}

	void CMultiManager::delFileEvent(INT fd, INT mask)
	{
		CHECK( UINT(fd) < s_pEventLoop->uiSize );

		if ( nullptr != m_pBase ) 
			m_pBase->delFileEvent(fd, mask, s_pEventLoop);

		FILE_EVENT& event = s_pEventLoop->lstFileEvent[fd];
		event.mask = event.mask & (~mask);

		if ( event.mask == NET_NONE) {
			//clear file_event
			if ( event.clientData != nullptr ) {
				delete event.clientData;
				event.clientData = nullptr;
			}

			event.readProc 	= nullptr;
			event.writeProc 	= nullptr;
		}
	}

	INT CMultiManager::eventLoop(void* timeout)
	{
		if ( nullptr != m_pBase ) 
			return m_pBase->eventLoop(timeout, m_lstFired);
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

			s_pEventLoop->uiSize 		= size;
			s_pEventLoop->lstFileEvent 	= ::std::vector<tagFileEvent>(size);
			s_pEventLoop->lstTimeEvent 	= ::std::vector<tagTimeEvent>(MAX_TIME_EVENT_SIZE);
			
			//init new items
			for ( UINT index=0; index < size; ++index ) {
				s_pEventLoop->lstFileEvent[index].mask 			= NET_NONE;	
				s_pEventLoop->lstFileEvent[index].clientData 	= nullptr;
				s_pEventLoop->lstFileEvent[index].readProc 		= nullptr;
				s_pEventLoop->lstFileEvent[index].writeProc 	= nullptr;
			}

		} else {
			UINT oldSize = s_pEventLoop->uiSize;
			CHECK_R( size > oldSize, oldSize );			
			
			s_pEventLoop->uiSize = size;
			s_pEventLoop->lstFileEvent.resize(size);	

			for ( UINT index=oldSize; index < size; ++index ) {
				s_pEventLoop->lstFileEvent[index].mask 			= NET_NONE;
				s_pEventLoop->lstFileEvent[index].clientData 	= nullptr;
				s_pEventLoop->lstFileEvent[index].readProc 		= nullptr;
				s_pEventLoop->lstFileEvent[index].writeProc 	= nullptr;
			}
		}
		return s_pEventLoop->uiSize;
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
