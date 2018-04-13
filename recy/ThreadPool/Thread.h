#ifndef __THREAD_H__
#define __THREAD_H__

#include <mutex>
#include <thread>
#include <condition_variable>
#include "Memory.h"

namespace Net
{
	virtual class ITask
	{
		public:
			virtual void start() 			= 0;
			virtual void stop()				= 0;
			virtual void pause()			= 0;
			virtual void resume()			= 0;
	};

	virtual class IThreadListener
	{
		public:
			virtual void prePend() = 0;
			virtual void preBlock() = 0;
			virtual void preDestroy() = 0;
	};

	class CThread 
	{
		public:
			enum EThreadStatus {
				ETS_Uninitialized,
				ETS_Idle,
				ETS_Pending,
				ETS_Block,
				ETS_Working,
				ETS_Destroy,
			};

		public:
			CThread();
			virtual ~CThread();

			void setTask(ITask* pTask);

			void initialize();
			void run();			
			void pend(unsigned int msec);
			void block();
			void destroy();

			inline EThreadStatus status()	{ return m_eStatus;	}

		private:
			void switchStatus(EThreadStatus m_eStatus);
			void setSleepSpan(unsigned int msec) { m_uiSleepSpan = msec; }

		private:
			EThreadStatus 				m_eStatus;
			CMemory<ITask> 				m_pTask;
			
			bool		 				m_bIsAlive;
			unsigned int 				m_uiSleepSpan;

			std::thread					m_thread;
			std::mutex					m_mutex;
			std::condition_variable		m_cond;
	};

	class CThreadManager 
	{
		public:

		private:
			friend class CThread;
			std::vector<std::thread> m_lstThreadsIdle;
	};
}
#endif
