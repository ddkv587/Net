#include "Thread.h"

namespace Net
{
	CThread::CThread()
		: m_eStatus(ETS_Uninitialized)
		, m_pTask()
		, m_mutex()
	{
		m_thread = std::thread(&Thread::run, this);
		m_thread.detach();
	}

	virtual CThread::~CThread()
	{

	}

	void CThread::setTask(ITask* pTask)
	{

	}

	void CThread::initialize()
	{

	}

	void CThread::run()
	{
	}

	void CThread::pend()
	{
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
