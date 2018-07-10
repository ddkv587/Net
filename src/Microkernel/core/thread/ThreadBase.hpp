#ifndef __THREADBASEHPP__
#define __THREADBASEHPP__

namespace NET
{
	class CThreadBase
	{
	public:
		enum EPolicy
		{
			SCHED_OTHER = 0,
			SCHED_BATCH,
			SCHED_IDLE,
			SCHED_FIFO,
			SCHED_RR
		};

	public:
		CThreadBase()
			: m_bStop(FALSE)
			, m_iPriority(0)
		{
			;
		}

		virtual ~CThreadBase() 
		{
			if ( m_thread.joinable() ) {
				m_thread.join();
			}
		}

		void run(void* arg = nullptr)
		{
			m_thread = ::std::thread(&CThreadBase::mainLoop, this, std::move(arg));
		}

		void stop()
		{
			m_bStop = TRUE;
		}

		::std::thread::id getThreadID()
		{
		 	return m_thread.get_id();
		}

		// ============= priority ===================
		static ::std::tuple<INT, INT>		range(EPolicy policy);				
		BOOLEAN 							setPriority(INT iPriority, EPolicy policy);			
		UINT 								priority();															

		CThreadBase(CThreadBase&) = delete;
		CThreadBase(const CThreadBase&) = delete;
		CThreadBase& operator=(const CThreadBase&) = delete;

	protected:
		virtual void mainLoop(void* arg)
		{
			UNUSED(arg);
			while ( !m_bStop ) {
				::std::this_thread::sleep_for(::std::chrono::milliseconds(1000));
			}
		}

	private:
		INT									getPolicy(EPolicy policy);

	protected:
		BOOLEAN 		m_bStop;
		UINT 			m_iPriority;

	private:
		::std::thread 	m_thread;
	};
}
#endif