#include "Thread.h"

namespace Net
{
	CThread::CThread()
		: m_eStatus(ETS_Uninitialized)
		, m_bIsAlive(true)
		, m_uiSleepSpan(1000)
	{
		m_thread = std::thread(&Thread::run, this);
		m_thread.detach();
	}

	virtual CThread::~CThread()
	{

	}

	void CThread::setTask(ITask* pTask)
	{
		if ( ETS_Idle == m_eStatus ) {
			if ( nullptr != m_pTask ) {
				m_pTask->stop();
			}
			m_pTask = pTask;
		}
	}

	void CThread::initialize()
	{
		
	}

	void CThread::run()
	{
		std::unique_lock<std::mutex> locker(m_mutex);
		while ( m_bIsAlive ) {
			while ( m_bIsAlive && !locker.try_lock_for(std::chrono::milliseconds(m_uiSleepSpan)) );
			if ( !m_bIsAlive ) break;

			if ( nullptr != m_pTask && ETS_Idle == m_eStatus ) {
				m_eStatus = ETS_Working;
				locker.unlock();
				m_pTask->start();
			}	
			locker.unlock();
		}
		if ( locker )  
			locker.unlock();
	}

	void CThread::pend(unsigned int msec)
	{
		std::this_thread::yield();
	}

	void CThread::block()
	{
	}

	void CThread::destroy()
	{
	}

	void CThread::switchStatus(EThreadStatus m_eStatus)
	{
	}
}
