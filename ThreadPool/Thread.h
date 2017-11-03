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
			virtual void run() = 0;
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
			void pend();
			void block();
			void destroy();

			inline EThreadStatus status()	{ return m_eStatus;	}

		private:
			void switchStatus(EThreadStatus m_eStatus);

		private:
			EThreadStatus 				m_eStatus;
			CMemory<ITask> 				m_pTask;
			
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
	};
}
#endif
