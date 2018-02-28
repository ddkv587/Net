#include "Multiplex.hpp"

CMultiplexManager::CMultiplexManager(EMultip_Type type, int size)
	: m_pEventLoop(NULL)
	, m_pHandler(NULL)
{
	m_pEventLoop = new EVENT_LOOP(size);

	switch(type) {
	case EMT_EPOLL:
		m_pHandler = new MultiEpoll();
		break;
	case EMT_KQUEUE:
		m_pHandler = new MultiKqueue(size);
		break;
	case EMT_SELECT:
		m_pHandler = new MultiSelect();
		break;
	case EMT_NONE:
		break;
	}
	assert(NULL != m_pEventLoop && NULL != m_pHandler);
}

CMultiplexManager::~CMultiplexManager()
{
	delete m_pEventLoop;
	delete m_pHandler;
}

void CMultiplexManager::resizeEvents(int size)
{
	if ( m_pEventLoop->size == size || \
			m_pEventLoop->maxfd > size )
		return;

	assert(NULL != m_pEventLoop);

	delete m_pEventLoop->event; 

	assert(NULL != m_pHandler);
	m_pHandler->resize(size);
}

void CMultiplexManager::switchType(EMultip_Type)
{

}

void CMultiplexManager::addFileEvent(int fd, int mask)
{
	assert(NULL != m_pHandler);

	m_pHandler->addFileEvent(fd, mask);
}

void CMultiplexManager::delFileEvent(int fd, int mask)
{
	assert(NULL != m_pHandler);

	m_pHandler->delFileEvent(fd, mask);
}

int CMultiplexManager::eventLoop(void* timeout)
{
	assert(NULL != m_pHandler);

	m_pHandler->eventLoop(m_pEventLoop, timeout);
}
