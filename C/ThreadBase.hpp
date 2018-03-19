#ifndef __THREADBASEHPP__
#define __THREADBASEHPP__

#include <thread>
namespace NET
{
	class ThreadBase
	{
	public:
		ThreadBase()
			: m_bStop(false)
		{
			;
		}

		virtual ~ThreadBase() 
		{
			if ( m_thread.joinable() ) {
				m_thread.join();
			}
		}

		void run(void* arg = NULL)
		{
			m_thread = std::thread(&ThreadBase::mainLoop, this, std::move(arg));
		}

		void stop()
		{
			m_bStop = true;
		}

		std::thread::id getThreadID()
		{
		 	return m_thread.get_id();
		}

		ThreadBase(ThreadBase&) = delete;
		ThreadBase(const ThreadBase&) = delete;
		ThreadBase& operator=(const ThreadBase&) = delete;

	protected:
		virtual void mainLoop(void* arg)
		{
			while ( !m_bStop ) {
				;
			}
		}

	protected:
		bool 		m_bStop;

	private:
		std::thread m_thread;
	};
}
#endif
