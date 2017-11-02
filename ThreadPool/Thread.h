#ifndef __THREAD_H__
#define __THREAD_H__

#include "Memory.h"

class CThread
{
	public:
		enum EThreadStatus {
			ETS_Alive,
			ETS_Uninitialized,
			ETS_Idle,
			ETS_Block,
			ETS_Working,
			ETS_Destroy,
		};

	public:
		CThread();
		virtual ~CThread();

		inline EThreadStatus getStatus()	{ return m_eStatus;	}

	private:
		void switchStatus(EThreadStatus m_eStatus);
	
	private:
		EThreadStatus m_eStatus;
};

class CThreadPool
{
	
	private:
}
#endif
