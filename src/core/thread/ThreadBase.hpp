#ifndef __THREADBASEHPP__
#define __THREADBASEHPP__

namespace NET
{
	class CThreadBase
	{
	public:
		CThreadBase()
			: m_bStop(false)
			, m_strName("Unknow")
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
			m_thread = ::std::thread(&CThreadBase::mainLoop, this, ::std::move(arg));
		}

		void stop()
		{
			m_bStop = true;
		}

		::std::thread::id getThreadID()
		{
		 	return m_thread.get_id();
		}

		void setName(::std::string strName)
		{
			m_strName = strName;
		}
		
		::std::string getName()
		{
			return m_strName; 
		}

		virtual void setPriority(int pri)
		{
			int temp;
			sched_param sch;
			pthread_getschedparam(m_thread.native_handle(), &temp, &sch);

			sch.sched_priority = pri;
			pthread_setschedparam(m_thread.native_handle(), SCHED_FIFO, &sch);	//FIFO is preset
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
		bool 			m_bStop;

	private:
		::std::string 	m_strName;
		::std::thread 	m_thread;
	};
}
#endif
