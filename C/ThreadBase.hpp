#ifndef __THREADBASEHPP__
#define __THREADBASEHPP__

#include <thread>
#include <utility>

namespace NET
{
	class ThreadBase
	{
	public:
		virtual ~ThreadBase() 
		{
			if ( m_thread.joinable() ) {
				m_thread.join();
			}
		}

		void run(void* arg = NULL)
		{
			m_thread = std::thread(mainLoop, this, arg);
		}

		std::thread::id getThreadID()
		{
		 	return m_thread.get_id();
		}

		ThreadBase() = delete;
		ThreadBase(ThreadBase&) = delete;
		ThreadBase(const ThreadBase&) = delete;
		ThreadBase& operator=(const ThreadBase&) = delete;

	protected:
		virtual void mainLoop(void* arg) = 0;

	private:
		std::thread m_thread;
	};
}
#endif
