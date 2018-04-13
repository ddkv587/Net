#ifndef __THREADBASEHPP__
#define __THREADBASEHPP__

namespace NET
{
	class CThreadBase
	{
	public:
		CThreadBase()
			: m_bStop(false)
		{
			;
		}

		virtual ~CThreadBase() 
		{
			if ( m_thread.joinable() ) {
				m_thread.join();
			}
		}

		void run(void* arg = NULL)
		{
			m_thread = ::std::thread(&CThreadBase::mainLoop, this, std::move(arg));
		}

		void stop()
		{
			m_bStop = true;
		}

		::std::thread::id getThreadID()
		{
		 	return m_thread.get_id();
		}

		CThreadBase(CThreadBase&) = delete;
		CThreadBase(const CThreadBase&) = delete;
		CThreadBase& operator=(const CThreadBase&) = delete;

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
		::std::thread m_thread;
	};
}
#endif
